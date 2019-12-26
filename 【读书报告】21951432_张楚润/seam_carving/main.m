clc; clear all; close all;
%%

image = imread('pic1.png');
image = imresize(image, 0.5);
figure;
imshow(image);
title('Original RGB Image');
fprintf('Size of the Original Image: %d x %d\n',size(image,1), size(image,2));
%%

% 调用SeamCarving函数，传入参数为图像和要裁剪的列数
OutPut = SeamCarving(image, 50);

%%

% 显示结果
figure;
imshow(OutPut);
title('Image After seam Carving');
fprintf('Size of the seam Carved Image: %d x %d\n',size(OutPut,1),size(OutPut,2));