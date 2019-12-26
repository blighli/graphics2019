function SeamCarvedIm  = SeamCarving( I, cols )
%% 
% 删除掉输入图像的指定列数

SeamCarvedIm = I; % Assign initial image as SeamCarved Image.
%%
for i = 1:cols    % 寻找纵向能量线
    energy = energyIm(SeamCarvedIm); % 求能量图
    M = seamImage(energy);%求累积能量值
    seam = backTrack(M); %寻找能量线经过的位置
    SeamCarvedIm = removeSeam(SeamCarvedIm,seam); %移除能量线
end