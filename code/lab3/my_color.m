%read the image
I = imread('burst.jpg');	
imshow(I);
		
%Extract RED, GREEN and BLUE components from the image
R = I(:,:,1);			
G = I(:,:,2);
B = I(:,:,3);

R = floor(R/(256/(2^3)));
G = floor(G/(256/(2^3)));
B = floor(B/(256/(2^2)));

COLOR = R.*2^(2+3) + G.*2^2 + B;

fileID = fopen ('burst.list', 'w');
for i = 1:size(COLOR(:), 1)-1
    fprintf (fileID, '%x\n', COLOR(i)); % COLOR (dec) -> print to file (hex)
end
fprintf (fileID, '%x', COLOR(size(COLOR(:), 1))); % COLOR (dec) -> print to file (hex)
fclose (fileID);
