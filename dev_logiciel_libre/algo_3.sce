img = imread('test_images/lena.png');

sobel_h = [-1 -2 -1; 0 0 0; 0 1 0];
sobel_v = [-1 0 1; -2 0 2; -1 0 1];

laplacien_4 = [0 1 0; 1 -4 1; 0 1 0];
laplacien_8 = [1 1 1; 1 -8 1; 1 1 1];

res1 = filter2(img, sobel_v);
res2 = filter2(img, sobel_h);

res3 = filter2(img, laplacien_4);
res4 = filter2(img, laplacien_8);

subplot(2,3,1); imshow(img); title("img");
subplot(2,3,2); imshow(res1); title("img");
subplot(2,3,3); imshow(res2); title("img");

subplot(2,3,4); imshow(img); title("img");
subplot(2,3,5); imshow(res3); title("img");
subplot(2,3,6); imshow(res4); title("img");
