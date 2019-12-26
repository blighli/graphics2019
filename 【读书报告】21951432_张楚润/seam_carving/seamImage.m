%计算累积能量值
function seamimage = seamImage(E)

sz = size(E); % Get the size of the Energy map.

seamimage = zeros(sz(1),sz(2));
seamimage(1,:) = E(1,:);%取能量矩阵的第一行元素

for i=2:sz(1)
    for j=1:sz(2)
        % 控制好边缘线
        if j-1<1
            seamimage(i,j)= E(i,j)+min([seamimage(i-1,j),seamimage(i-1,j+1)]);%位于最左侧
        elseif j+1>sz(2)
            seamimage(i,j)= E(i,j)+min([seamimage(i-1,j-1),seamimage(i-1,j)]);%位于最右侧
        else
            seamimage(i,j)= E(i,j)+min([seamimage(i-1,j-1),seamimage(i-1,j),seamimage(i-1,j+1)]);
        end
    end
end