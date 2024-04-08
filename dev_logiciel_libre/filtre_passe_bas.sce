img = imread('test_images/daf.tiff');
y = imnoise(img, 'gaussian'); //gaussian
z = imnoise(img, 'salt & pepper');

// Filtre maoyenneur
k = [1/9 1/9 1/9; 1/9 1/9 1/9; 1/9 1/9 1/9];

res1 = filter2(y, k);
res1 = uint8(res1);

res2 = filter2(z, k);
res2 = uint8(res2);

subplot(2,2,1); imshow(y); title("Image with Gaussin noise");
subplot(2,2,2); imshow(res1); title("res1");
subplot(2,2,3); imshow(z); title("Image with Salt and Pepper noise");
subplot(2,2,4); imshow(res2); title("res2");
