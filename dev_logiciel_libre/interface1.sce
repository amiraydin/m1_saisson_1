f=figure('figure_position',[100 100],'figure_size',[1200,700],'auto_resize','on','background',[-2],'figure_name','Interface Scilab','dockable','off','infobar_visible','off','toolbar_visible','off','menubar_visible','on','default_axes','on','visible','off');

frame_height = 10;
f.figure_id = 77;
f.immediate_drawing = "on";
f.visible="on";

image = uimenu(f, "label", "Image");
hist = uimenu(f, "label", "Histgram");
filtre = uimenu(f, "label", "Filtre");
//telecharger image
uimenu(image, "label", "Telecharger", "callback", "tel_image_one()", "tag", "tel_img");
// parti histogram
uimenu(hist, "label", "Histogram", "callback", "histo_img()", "tag", "histo");
uimenu(hist, "label", "Egalisation", "callback", "histo_egal()", "tag", "his_egal");
uimenu(hist, "label", "Extension linéaire", "callback", "histo_lin()", "tag", "his_lin");
// parti filtres
uimenu(filtre, "label", "Gaussien", "callback", "gaussien()", "tag", "gaus");    
uimenu(filtre, "label", "Salt & Pepper", "callback", "salt_pepper()", "tag", "sal");
uimenu(filtre, "label", "laplacien 4", "callback", "laplacien_4()");
uimenu(filtre, "label", "laplacien 8", "callback", "laplacien_8()");
uimenu(filtre, "label", "sobel horizentale", "callback", "sobel_h()");
uimenu(filtre, "label", "sobel verticale", "callback", "sobel_v()");
uimenu(filtre, "label", "sobel", "callback", "sobel_v_h()");
uimenu(filtre, "label", "Original", "callback", "origin()");

uimenu(f, "label", "Aide", "callback", "gethelp()", "tag", "aide");
uimenu(f, "label", "Sortir", "callback", "sortir()", "tag", "sort");
f.userdata = struct('results', []);

function tel_image_one()
    f = gcf();
    img_path = uigetfile(["*.tiff";"*.bmp";"*.png"], "C:/Users/Amir/Desktop/paris8/dev_logiciel_libre/test_images","Choose a file");
    img = imread(img_path);
    f.userdata.results = [img];
    imshow(img);
    
endfunction

function histo_img()
    f.userData.results=[];
    img_path = uigetfile(['*.bmp';'*.png'], "C:/Users/Amir/Desktop/paris8/dev_logiciel_libre/test_images","Choose a file");
    disp(img_path);
    x=imread(img_path);
    [l,c] = size(x);
    // Calcul de l'histogramme
    h=zeros(1,256)
    for i = 1:l
        for j = 1:c
            h(x(i,j)+1)=h(x(i,j)+1)+1
        end
    end
    subplot(1,3,1), plot(h);
    subplot(1,3,2), bar(imhist(x));
    subplot(1,3,3), imshow(x);
endfunction

function histo_egal()
    f.userData.results=[];
    img_path = uigetfile(['*.tiff';'*.bmp';'*.png'],"C:/Users/Amir/Desktop/paris8/dev_logiciel_libre/test_images","Choose a file");
    disp(img_path);
    x=imread(img_path);
    h=zeros(1,256);
    [l,c] = size(x);
    for i = 1:l
        for j = 1:c
            h(x(i,j)+1)=h(x(i,j)+1)+1
        end
    end

    k=size(h,2)
    for i = 1:k
        somme = 0
        for j = 1:i
            somme = somme + h(j)
        end
        C(i) = somme
    end

    y=x;
    for i = 1:l
        for j = 1:c
            y(i,j)= 255*C(x(i,j))/(l*c);
        end
    end
    subplot(2,2,1), imshow(x), title('originale');
    subplot(2,2,2), bar(imhist(x));
    subplot(2,2,3), imshow(y), title('égalisation d''histogramme');
    subplot(2,2,4), bar(imhist(y));
endfunction

function histo_lin()
    f.userData.results=[];
    img_path = uigetfile(['*.tiff';'*.bmp'], "C:/Users/Amir/Desktop/paris8/dev_logiciel_libre/test_images","Choose a file");
    disp(img_path);
    x=imread(img_path);
    [l,c] = size(x);
    y=x;

    for i = 1:l
        for j = 1:c
            y(i,j)= (255/(max(x)-min(x)))*(x(i,j) - min(x));
        end
    end
    subplot(1,2,1), imshow(x), title('originale');
    subplot(1,2,2), imshow(y), title('Histogram linéaire de dynamique');
endfunction

// Filtres
function gaussien()
    x=f.userData.results;
    y = imnoise(x, 'gaussian');
    //user_input = input("Entrez une information :", "s");
    k=[1/8 1/8 1/8; 1/8 1/8 1/8; 1/8 1/8 1/8];
    res1 = filter2(y,k);
    res1=uint8(res1);
    imshow(y); 
endfunction

function salt_pepper()
    x=f.userData.results;
    z = imnoise(x, 'salt & pepper');
    k=[1/9 1/9 1/9; 1/9 1/9 1/9; 1/9 1/9 1/9];
    res2 = filter2(z,k);
    res2=uint8(res2);
    imshow(res2);
endfunction

function laplacien_4()
    x=f.userData.results;
    laplacien_4_f = [0 1 0; 1 -4 1; 0 1 0];
    res1 = filter2(x,laplacien_4_f);
    imshow(res1);
endfunction

function laplacien_8()
     x=f.userData.results;
     laplacien_8_f = [1 1 1; 1 -8 1; 1 1 1];
     res2 = filter2(x,laplacien_8_f);
     imshow(res2);
endfunction

function sobel_h()
     x=f.userData.results;
     sobel_h_filtre = [-1 -2 -1; 0 0 0; 1 2 1];
     res3 = filter2(x,sobel_h_filtre);
     imshow(res3);
endfunction

function sobel_v()
     x=f.userData.results;
     sobel_v_f = [-1 0 1; -2 0 2; -1 0 1];
     res4 = filter2(x, sobel_v_f);
     imshow(res4);
endfunction

function sobel_v_h()
    x= f.userData.results;
    sobel_h_f = [-1 -2 -1; 0 0 0; 1 2 1];
    sobel_v_f = [-1 0 1; -2 0 2; -1 0 1];
    res3 = filter2(x,sobel_v_f);
    res4 = filter2(x,sobel_h_f);
    res5=uint8(sqrt(res3.^2 +res4.^2));
    imshow(res5);
endfunction

function origin()
    x=f.userData.results;
    imshow(x);
endfunction

function gethelp()
    messagebox("Interface Utilisateur Scilab, Image : Télècharger une image depuis votre ordinateur puis vous pouvez le filtrer depuis onglet Filtre. Histogram : selectionner le histograme pour telecharger votre image et voir le resultat.", "info", "Information", "modal");
endfunction

function sortir()
    close()
endfunction

function ReduireEchelle_callback(handles)
    ///% Récupérez les données de l'image depuis l'interface
    axes_image = handles.Axes1;
    img = axes_image.children(1).data;
   /// % Vérifiez si l'image a déjà été réduite
    if isfield(handles, 'image_reduite') && handles.image_reduite
        disp('image a déjà été réduite. Vous ne pouvez pas la réduire davantage.')
    else
        [h, w, c] = size(img);
        facteur_reduction = 0.5;
        new_h = round(h * facteur_reduction);
        new_w = round(w * facteur_reduction);
        S_reduite = imresize(img, [new_h, new_w]);
        axes_image.children(1).data = S_reduite;
        title('Image réduite');
        handles.image_reduite = true;
        
        handles.image_reduite = %T;
        guidata(handles.figure1, handles);
    end
end  
