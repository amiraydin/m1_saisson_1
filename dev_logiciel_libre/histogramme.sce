//x=imread('lena.png');
x=imread('test_images/house.png');
[l,c] = size(x);

// Calcul de l'histogramme
/*
h=zeros(1,256)
for i = 1:l
    for j = 1:c
        h(x(i,j)+1)=h(x(i,j)+1)+1
    end
end

subplot(1,3,1), plot(h);
subplot(1,3,2), bar(imhist(x));
subplot(1,3,3), imshow(x);
*/

// Méthode 1: Extension linéaire de dynamique 
/*
y=x;

for i = 1:l
    for j = 1:c
        y(i,j)= (255/(max(x)-min(x)))*(x(i,j) - min(x));
    end
end

subplot(1,2,1), imshow(x), title('image orginale');
subplot(1,2,2), imshow(y), title('image avec extension linéaire de dynamique');
*/

// Méthode 2: Egalisation de l'histogramme

// Etape 1
h=zeros(1,256)
for i = 1:l
    for j = 1:c
        h(x(i,j)+1)=h(x(i,j)+1)+1
    end
end
// Etape 2
k=size(h,2)
for i = 1:k
    somme = 0
    for j = 1:i
        somme = somme + h(j)
    end
    C(i) = somme
end
// Etape 3
y=x;
for i = 1:l
    for j = 1:c
        y(i,j)= 255*C(x(i,j))/(l*c);
    end
end

subplot(2,2,1), imshow(x), title('image orginale');
subplot(2,2,2), bar(imhist(x));
subplot(2,2,3), imshow(y), title('image avec égalisation d''histogramme');
subplot(2,2,4), bar(imhist(y));







