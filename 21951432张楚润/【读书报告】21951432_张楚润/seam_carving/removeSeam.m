function x=removeSeam(x,Seam)
[rX, cX, dim] = size(x);
[Sr, Sc, ~] = size(Seam); 

if rX~=Sr
        error('SeamVector and image dimension mismatch');
end
%ReduceImage = zeros(size(x));
if (rX == Sr)
    for k=1:Sc              %删除Sc组能量线
        for i=1:dim             %对于RGB图，删除能量点的每个通道值if rgb, goes through each channel
            for j=1:rX        %图像有rX行
                if Seam(j,k)==1
                    ReduceImage(j,:,i)= x(j,2:cX,i);%能量点位于最左侧
                elseif Seam(j,k)==cX
                    ReduceImage(j,:,i)= x(j,1:cX-1,i);%能量点位于最右侧
                else
                    ReduceImage(j,:,i)=[x(j,1:Seam(j,k)-1,i) x(j,Seam(j,k)+1:cX,i)];
                end
            end
        end
        x=ReduceImage;
        clear ReduceImage
        [rX, cX, dim]=size(x);
    end
end