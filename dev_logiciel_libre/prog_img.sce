img = imread('house.png');
[x, y] = size(img);
// Calcul de hisogramme 
h = zeros(1, 255)
for i = 1:x 
    for j = 1:y
        h(img(i,j))=h(img(i,j))+1
    end
end

subplot(1, 3, 1), plot(h);
subplot(1, 3, 2), bar(imhist(img));
subplot(1, 3, 3), imshow(img);
