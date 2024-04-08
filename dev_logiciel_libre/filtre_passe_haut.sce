img = imread('lena.png');

laplacien_4 = [0 1 0; 1 -4 1; 0 1 0];
laplacien_8 = [1 1 1; 1 -8 1; 1 1 1];

sobel_h = [-1 -2 -1; 0 0 0; 1 2 1];
sobel_v = [-1 0 1; -2 0 2; -1 0 1];

res1 = filter2(img,laplacien_4);
res2 = filter2(img,laplacien_8);

res3 = filter2(img,sobel_v);
res4 = filter2(img,sobel_h);
res5=uint8(sqrt(res3.^2 +res4.^2));

subplot(2,3,1); imshow(img); title('image orginale');
subplot(2,3,2); imshow(res1); title('Laplacien-4');
subplot(2,3,3); imshow(res2); title('Laplacien-8');

subplot(2,3,4); imshow(res3); title('Sobel V');
subplot(2,3,5); imshow(res4); title('Sobel H');
subplot(2,3,6); imshow(res5); title('Module du gradient de Sobel');


