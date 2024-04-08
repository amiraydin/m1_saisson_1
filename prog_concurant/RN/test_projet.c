#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <pthread.h>

#define numEntree 2
#define numCoucheCache 2
#define numSortie 1
#define epochs 4

struct ParametresRNA {
    double pas;
    int nb_entrainement;
    double entree_combinaison[epochs][numEntree];
    double sortie_xor[epochs][numSortie];
    // Ajoute d'autres paramètres si nécessaire
};

// Fonction d'activation
double sigmoid(double x) { return 1 / (1 + exp(-x)); }
double derive_sigmoid(double x) { return x * (1 - x); }
double tanh_hyperbolique (double x) { return tanh(x); }

// Fonction initie un poids aleatoire
double init_poids() { return ((double)rand()) / ((double)RAND_MAX); }

void init_poids_couche_sortie(double (*ccp)[numCoucheCache], double (*csp)[numSortie]) {
    for(int i = 0; i< numEntree; i++){
            for(int j = 0; j< numCoucheCache; j++){
                ccp[i][j] = init_poids();
            }
        }

    for(int i = 0; i< numCoucheCache; i++){
        for(int j = 0; j< numSortie; j++){
            csp[i][j] = init_poids();
        }
    }
}

void init_biais_couche(double (*csb)) {
    for(int i = 0; i< numSortie; i++){
            csb[i] = init_poids();
    }
}

void afficher_poids_couche_cache(double (*ccp)[numCoucheCache]){
    for(int i = 0; i < numEntree; i++){
        for(int j = 0; j< numCoucheCache; j++){
            printf("coucheCachePoids[%d][%d] = %lf \n", i, j, ccp[i][j]);
        }
    }
}

void afficher_biais_couche_cache_sortie(double (*ccs)) {
    for(int i = 0; i< numSortie; i++){
         printf("coucheSortieBiais[%d] = %lf \n", i,  ccs[i]);
    }
}

double propagation_avant(int i,
                         double pas,
                         double (*entrainement_entree)[numEntree], 
                         double (*coucheCacheBiais), 
                         double (*coucheCachePoids)[numCoucheCache],
                         double (*coucheCache), 
                         double (*coucheSortieBiais), 
                         double (*coucheSortiePoids)[numSortie], 
                         double (*coucheSortie)) { 
    // Activation couche cachee
    for(int j = 0; j < numCoucheCache; j++){
        double activation = coucheCacheBiais[j];
        for(int k = 0; k < numEntree; k++){
            activation += entrainement_entree[i][k] * coucheCachePoids[k][j];
        }
        coucheCache[j] = tanh_hyperbolique(activation);
    }
    
    // Activation couche sortie
    for(int j = 0; j < numSortie; j++){
        double activation = coucheSortieBiais[j];
        for(int k = 0; k < numCoucheCache; k++){
            activation += coucheCache[k] * coucheSortiePoids[k][j];
        }
        coucheSortie[j] = tanh_hyperbolique(activation);
    }

}

double propagation_arriere(int i,
                           double pas,
                           double (*entrainement_entree)[numEntree], 
                           double (*entrainement_sortie)[numSortie],
                           double (*coucheCacheBiais), 
                           double (*coucheCachePoids)[numCoucheCache],
                           double (*coucheCache), 
                           double (*coucheSortieBiais), 
                           double (*coucheSortiePoids)[numSortie], 
                           double (*coucheSortie)) { 

    double deltaSortie[numSortie];
            
    // Corriger les couches sortie 
    for(int j = 0; j < numSortie; j++){
        double erreur = (entrainement_sortie[i][j] - coucheSortie[j]);
        deltaSortie[j] = erreur * derive_sigmoid(coucheSortie[j]);
    }

    // Corriger les couches cachee
    double deltaCache[numCoucheCache];
    for(int j = 0; j < numCoucheCache; j++){
        double erreur = 0.0f;
        for(int k = 0; k < numSortie; k++){
            erreur += deltaSortie[k] * coucheSortiePoids[j][k];
            // printf(" k = %d   j = %d\n ", k,j);
            // printf(" deltaSortie[%d] = %lf | coucheSortiePoids[%d][%d] = %lf\n", k, deltaSortie[k], j, k, coucheSortiePoids[j][k]);
        }
        deltaCache[j] = erreur * derive_sigmoid(coucheCache[j]);
    }

    // Mettre a jours le poids de la couche sortie
    for(int j = 0; j < numSortie; j++){
        coucheSortieBiais[j] += deltaSortie[j] * pas;
        for(int k = 0; k < numCoucheCache; k++){
            coucheSortiePoids[k][j] += coucheCache[k] * deltaSortie[j] * pas;
            // printf("CoucheSortiePoids[%d][%d] = %lf \n", j, k, coucheSortiePoids[k][j]);

        }
    }

    // Mettre a jours le poids de la couche cachee
    for(int j = 0; j < numCoucheCache; j++){
        coucheCacheBiais[j] += deltaCache[j] * pas;
        for(int k = 0; k < numEntree; k++){
            coucheCachePoids[k][j] += entrainement_entree[i][k] * deltaCache[j] * pas;
        }
    }
}

void melange(int *array, size_t n) {
    if(n > 1) {
        size_t i;
        for (i = 0; i < n; i++) {
            size_t j = i + rand() / (RAND_MAX / (n - i) + 1);
            int t = array[j];
            array[j] = array[i];
            array[i] = t; 
        }
    }
}

// double calculerPrecision(int nombreExemples,
//                          double (*entrainement_entree)[numEntree], 
//                          double (*coucheSortie),
//                          double (*entrainement_sortie)[numSortie]) {
//     int predictionsCorrectes = 0;

//     // Parcourir les sorties du modèle et comparer avec les sorties désirées
//     for (int i = 0; i < nombreExemples; ++i) {
//         printf("Sortie : %lf   Sortie désirée : %d  | ", coucheSortie[i], entrainement_sortie[i][0]);

//         // Comparer avec une tolérance (par exemple, considérer une sortie > 0.5 comme 1, sinon 0)
//         int predictionBinaire = (coucheSortie[i] > 0.5) ? 1 : 0;

//         printf("Précision : %d\n", (predictionBinaire == entrainement_sortie[i][0]));

//         if (predictionBinaire == entrainement_sortie[i][0]) {
//             predictionsCorrectes++;
//         }
//     }

//     // Calculer la précision en pourcentage
//     double precision = ((double)predictionsCorrectes / nombreExemples) * 100.0;

//     return precision;
// }

void entrainement(int nombreEpochs,
                  double pas,
                  double (*entrainement_entree)[numEntree], 
                  double (*entrainement_sortie)[numSortie],
                  double (*coucheCacheBiais), 
                  double (*coucheCachePoids)[numCoucheCache],
                  double (*coucheCache), 
                  double (*coucheSortieBiais), 
                  double (*coucheSortiePoids)[numSortie], 
                  double (*coucheSortie)) {

    int entrainementOrdre[] = {0,1,2,3};

    for(int epoch = 0; epoch < nombreEpochs; epoch++){

        // Different combinaison d'ordre d'entreé
        melange(entrainementOrdre, epochs);
        if(epoch == nombreEpochs - 1) { 
            printf(" => EPOCH %d \n ", epoch );
        }

        for(int x = 0; x < epochs; x++){
            
            int i = entrainementOrdre[x];

            // Propagation avant

            propagation_avant(i,
                              pas,
                              entrainement_entree, 
                              coucheCacheBiais, 
                              coucheCachePoids,
                              coucheCache, 
                              coucheSortieBiais, 
                              coucheSortiePoids, 
                              coucheSortie);

            // int nombreExemples = sizeof(coucheSortie) / sizeof(coucheSortie[0]);
            // double precision = calculerPrecision(nombreExemples, entrainement_entree, coucheSortie, entrainement_sortie);
            // printf(" nombreExemples %d et precision %g \n", nombreExemples, precision);

            
            
        
            // Propagation arriere
            propagation_arriere(i,
                                pas, 
                                entrainement_entree, 
                                entrainement_sortie, 
                                coucheCacheBiais, 
                                coucheCachePoids,
                                coucheCache, 
                                coucheSortieBiais, 
                                coucheSortiePoids, 
                                coucheSortie);

           
            if(epoch == nombreEpochs - 1) {
                printf("      Entree : %g %g  Sortie: %g   Sortie desirer: %g\n",
                            entrainement_entree[i][0], entrainement_entree[i][1],
                            coucheSortie[0], entrainement_sortie[i][0]);

            }

        }
    }

    // fputs("\n Final Couche Cachee Poids\n [", stdout);
    // for(int j = 0; j < numCoucheCache; j++){
    //     fputs("[  ", stdout);
    //     for(int k = 0; k < numEntree; k++){
    //         printf("%f ", coucheCachePoids[k][j]);
    //     }
    //     fputs(" ]", stdout);
    // }

    // fputs("]\n Final Couche Cachee Biais\n [", stdout);
    // for(int j = 0; j < numCoucheCache; j++){
    //     printf("%f ", coucheCacheBiais[j]);
    // }

    // fputs("]\n Final Couche Sortie Poids\n [", stdout);
    // for(int j = 0; j < numSortie; j++){
    //     fputs("[  ", stdout);
    //     for(int k = 0; k < numCoucheCache; k++){
    //         printf("%f ", coucheSortiePoids[k][j]);
    //     }
    //     fputs(" ]", stdout);
    // }

    // fputs("]\n Final Couche Sortie Biais\n [", stdout);
    // for(int j = 0; j < numSortie; j++){
    //     printf("%f ", coucheSortieBiais[j]);
    // }

    // fputs("] \n", stdout);
    
}

void rna(double pas, int nb_entrainement, double (*entrainement_entree)[numEntree], double (*entrainement_sortie)[numSortie]) {

    double coucheCache[numCoucheCache];
    double coucheSortie[numSortie];

    double coucheCacheBiais[numCoucheCache];
    double coucheSortieBiais[numSortie];

    double coucheCachePoids[numEntree][numCoucheCache];
    double coucheSortiePoids[numCoucheCache][numSortie];

    
    // Initialiser les poids des couches cache et couche sortie et le biais sortie
    init_poids_couche_sortie(coucheCachePoids, coucheSortiePoids); 
    init_biais_couche(coucheSortieBiais);
   
    // afficher_poids_couche_cache(coucheCachePoids);
    // afficher_biais_couche_cache_sortie(coucheSortieBiais);

    // Entrainement du reseau avec 5000 epochs, repetition entrainement
    entrainement(nb_entrainement,
                pas, 
                entrainement_entree, 
                entrainement_sortie, 
                coucheCacheBiais, 
                coucheCachePoids,
                coucheCache, 
                coucheSortieBiais, 
                coucheSortiePoids, 
                coucheSortie);
}


void * rna_thread(struct ParametresRNA *params) {



    double pas = params->pas;
    int nb_entrainement = params->nb_entrainement;
    double (*entrainement_entree)[2] = params->entree_combinaison;
    double (*entrainement_sortie)[1] = params->sortie_xor;

    double coucheCache[numCoucheCache];
    double coucheSortie[numSortie];

    double coucheCacheBiais[numCoucheCache];
    double coucheSortieBiais[numSortie];

    double coucheCachePoids[numEntree][numCoucheCache];
    double coucheSortiePoids[numCoucheCache][numSortie];

    
    // Initialiser les poids des couches cache et couche sortie et le biais sortie
    init_poids_couche_sortie(coucheCachePoids, coucheSortiePoids); 
    init_biais_couche(coucheSortieBiais);
   
    // afficher_poids_couche_cache(coucheCachePoids);
    // afficher_biais_couche_cache_sortie(coucheSortieBiais);

    // Entrainement du reseau avec 5000 epochs, repetition entrainement

    printf("  Pas %g Nombre d'epochs: %d\n ", pas, nb_entrainement);

    entrainement(nb_entrainement,
                pas, 
                entrainement_entree, 
                entrainement_sortie, 
                coucheCacheBiais, 
                coucheCachePoids,
                coucheCache, 
                coucheSortieBiais, 
                coucheSortiePoids, 
                coucheSortie);

    
}

void *fonction_thread(void *args) {
    // Cast le pointeur générique en pointeur vers la structure ParametresRNA
    struct ParametresRNA *params = (struct ParametresRNA *)args;

    // Appelle la fonction rna avec les paramètres
    rna_thread(params);

    // Fin du thread
    pthread_exit(NULL);
}

int main(){

    pthread_t thread1, thread2;

    printf(" Projet reseau de neuronnes! \n");

    double pas = 0.1f;
    int nb_entrainement = 2000;

     // Entree pour l'entrainement Xor
    // double entree_combinaison[epochs][numEntree] = {{0.0f, 0.0f},
    //                                            {0.0f, 1.0f},
    //                                            {1.0f, 0.0f},
    //                                            {1.0f, 1.0f}};
                                               
    // double sortie_xor[epochs][numSortie] = {{0.0f},
    //                                            {1.0f},
    //                                            {1.0f},
    //                                            {0.0f}};

    // rna(pas, nb_entrainement, entree_combinaison, sortie_xor);

    // double sortie_and[epochs][numSortie] = {{0.0f},
    //                                            {0.0f},
    //                                            {0.0f},
    //                                            {1.0f}};

    // rna(pas, nb_entrainement, entree_combinaison, sortie_and);
                          

    // double sortie_or[epochs][numSortie] = {{0.0f},
    //                                            {1.0f},
    //                                            {1.0f},
    //                                            {1.0f}};

    // rna(pas, nb_entrainement, entree_combinaison, sortie_or);

    struct ParametresRNA parametre_xor = {
        .pas = 0.03,    
        .nb_entrainement = 5000,
        .entree_combinaison = {{0.0f, 0.0f},
                               {0.0f, 1.0f},
                               {1.0f, 0.0f},
                               {1.0f, 1.0f}}, 
        .sortie_xor = {{0.0f},
                       {1.0f},
                       {1.0f},
                       {0.0f}} 
    };

    struct ParametresRNA parametres_or = {
        .pas = 0.01,
        .nb_entrainement = 100,
        .entree_combinaison = {{0.0f, 0.0f},
                               {0.0f, 1.0f},
                               {1.0f, 0.0f},
                               {1.0f, 1.0f}}, 
        .sortie_xor = {{0.0f},
                       {0.0f},
                       {0.0f},
                       {1.0f}} 
    };

    pthread_mutex_t mutex;
           
	pthread_mutex_lock(&mutex);
    
    if( pthread_create(&thread1, NULL, fonction_thread, (void *)&parametre_xor) != 0) {
        perror("pthread_create");
        return 1;
    }	
    printf(" \n   [  XOR  ]   \n");
    pthread_join(thread1, NULL);
    
	pthread_mutex_unlock(&mutex);

    if( pthread_create(&thread2, NULL, fonction_thread, (void *)&parametres_or) != 0) {
        perror("pthread_create");
        return 1;
    }	
    printf(" \n   [  OR  ]   \n");
    pthread_join(thread2, NULL);

    pthread_mutex_destroy(&mutex);

    return 0;
}