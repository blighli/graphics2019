function seam = backTrack(x)
%返回最小能量线上各点的位置向量
[rows,cols] = size(x);

%从最下面一行像素点往上寻找能量线
for i=rows:-1:1 
    if i==rows
        [~,j]=min(x(rows,:));  %寻找最下面一行的最小能量点
    else %找出与前一行最小能量点相邻的三个像素点
        if seam(i+1)==1
            vec=[Inf x(i,seam(i+1)) x(i,seam(i+1)+1)];%位于最左侧一列
        elseif seam(i+1)==cols
            vec=[x(i,seam(i+1)-1) x(i,seam(i+1)) Inf];%位于最右侧一列
        else
            vec=[x(i,seam(i+1)-1) x(i,seam(i+1)) x(i,seam(i+1)+1)];
        end
        
        %在与前一行最小像素点相邻的三个像素点中，找出能量值最小的，并求其纵坐标
        [~,Inx]=min(vec);%返回1，2或3
        IndexInc=Inx-2;%-1，0或1
        j=seam(i+1)+IndexInc;%前一列，当前列或后一列
    end
    seam(i,1) = j;%第i行最小像素点的位置为(i, j)
end

end