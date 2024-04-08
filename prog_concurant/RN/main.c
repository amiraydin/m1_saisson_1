#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>

#include <pthread.h>

#define TAUX_APPRENTISSAGE 0.2

#define EPOCHES 10000

// Fonction d'activation sigmo�de
double sigmoid(double x)
{
    return 1 / (1 + exp(-x));
}

// D�riv�e de la fonction sigmo�de
double sigmoid_derivative(double x)
{
    return x * (1 - x);
}

// Fonction du r�seau neuronal pour la porte ET
int porte_et(double entree1, double entree2, double poids[2][2], double biais[2])
{
    double net[2];
    double sortie[2];

    // Calcul des sorties des neurones dans la couche cach�e
    for (int i = 0; i < 2; i++)
    {
        net[i] = entree1 * poids[0][i] + entree2 * poids[1][i] + biais[i];
        sortie[i] = sigmoid(net[i]);
    }

    // Calcul de la sortie finale
    double net_final = sortie[0] * poids[0][2] + sortie[1] * poids[1][2] + biais[2];
    double sortie_finale = sigmoid(net_final);

    return sortie_finale >= 0.5 ? 1 : 0;
}

void *lafunc(void *arg)
{

    // Initialisation du g�n�rateur de nombres al�atoires
    srand(time(NULL));

    double donnees_entree[4][2] = {{0, 0}, {0, 1}, {1, 0}, {1, 1}};
    // int donnees_sortie[4] = {0, 0, 0, 1};
    int *donnees_sortie = (int *)arg;

    // Initialisation des poids avec des valeurs al�atoires
    double poids[2][3];
    for (int i = 0; i < 2; i++)
    {
        for (int j = 0; j < 3; j++)
        {
            // poids[i][j] = (double)rand() / RAND_MAX;
            poids[i][j] = 2.0 * (((double)rand() / RAND_MAX) - 1.0);
        }
    }

    double biais[3];
    for (int i = 0; i < 3; i++)
    {
        // biais[i] = (double)rand() / RAND_MAX;
        biais[i] = 2.0 * (((double)rand() / RAND_MAX) - 1.0);
    }

    // Boucle d'apprentissage
    for (int epoch = 0; epoch < EPOCHES; epoch++)
    {
        double erreur_totale = 0.0;

        for (int i = 0; i < 4; i++)
        {
            double entree1 = donnees_entree[i][0];
            double entree2 = donnees_entree[i][1];
            int cible = donnees_sortie[i];

            // Calcul des sorties des neurones dans la couche cach�e
            double sortie[2];
            double net[2];

            for (int j = 0; j < 2; j++)
            {
                net[j] = entree1 * poids[0][j] + entree2 * poids[1][j] + biais[j];
                sortie[j] = sigmoid(net[j]);
            }

            // Calcul de la sortie finale
            double net_final = sortie[0] * poids[0][2] + sortie[1] * poids[1][2] + biais[2];
            double sortie_finale = sigmoid(net_final);

            double erreur = cible - sortie_finale;
            erreur_totale += erreur * erreur;

            // Mise � jour des poids et des biais avec la descente de gradient
            double delta_final = erreur * sigmoid_derivative(sortie_finale);

            for (int j = 0; j < 2; j++)
            {
                double delta_cache = delta_final * poids[j][2] * sigmoid_derivative(sortie[j]);
                poids[0][j] += TAUX_APPRENTISSAGE * delta_cache * entree1;
                poids[1][j] += TAUX_APPRENTISSAGE * delta_cache * entree2;
                biais[j] += TAUX_APPRENTISSAGE * delta_cache;
            }

            poids[0][2] += TAUX_APPRENTISSAGE * delta_final * sortie[0];
            poids[1][2] += TAUX_APPRENTISSAGE * delta_final * sortie[1];
            biais[2] += TAUX_APPRENTISSAGE * delta_final;
        }

        if (epoch % 1000 == 0)
        {
            printf("�poque %d - Erreur : %lf\n", epoch, erreur_totale);
        }
    }

    // Tester le r�seau entra�n�
    printf("Poids entra�n�s : \n");
    for (int i = 0; i < 2; i++)
    {
        for (int j = 0; j < 3; j++)
        {
            printf("%lf ", poids[i][j]);
        }
        printf("\n");
    }
    printf("Biais entra�n�s : \n");
    for (int i = 0; i < 3; i++)
    {
        printf("%lf ", biais[i]);
    }
    printf("\n");

    for (int i = 0; i < 4; i++)
    {
        double entree1 = donnees_entree[i][0];
        double entree2 = donnees_entree[i][1];
        int resultat = porte_et(entree1, entree2, poids, biais);
        printf("R�sultat de la porte pour %d %d : %d\n", (int)entree1, (int)entree2, resultat);
    }
    // pthread_exit(NULL);
}

int main()
{

    pthread_t thread1, thread2;

    int sort_et[4] = {0, 0, 0, 1};
    int sort_ou[4] = {0, 1, 1, 1};

    pthread_create(&thread1, NULL, lafunc, (void *)sort_et);
    pthread_create(&thread2, NULL, lafunc, (void *)sort_ou);

    pthread_join(thread1, NULL);
    pthread_join(thread2, NULL);

    printf("finitos");

    return 0;
}
