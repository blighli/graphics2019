% 能量图
function res = energyIm(I)
%对RGB三个通道的能量值进行求和
    res = energyGrey(I(:, :, 1)) + energyGrey(I(:, :, 2)) ...
        + energyGrey(I(:, :, 3));%为了简化计算，先转换成灰度图像
end

function res = energyGrey(I)
%求图像的梯度模值（能量图一般是图像像素的梯度模值）
res = abs(imfilter(I, [-1,0,1], 'replicate')) + ...
        abs(imfilter(I, [-1;0;1], 'replicate'));%再看看求梯度值的实现。算子...
end