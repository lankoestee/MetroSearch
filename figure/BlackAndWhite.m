% 读取图像数据和 alpha 通道
[image, ~, alpha] = imread('metroIcon2.png');

% 反色操作
image = abs(255 - image);

% 保存为新的 PNG 文件
imwrite(image, 'metroIcon3.png', 'Alpha', alpha);
