clear;clc;close all;
A = readmatrix("answer.txt");
col = length(A(1,:))-1;
row = length(A(:,1));
for i = 1:row
    hold on;
   plot((1:col),A(i,1:col),'-o','linewidth',3); 
end
set(gcf, 'Position',  [500 400 1000 400]);
axis([0 5 0 20]);
xlabel('associativity');  
ylabel('miss rate')  
set(gca,'Xtick',1:4);
set(gca,'Ytick',0:5:20)
set(gca,'XtickLabel',{'one-way','two-way','four-way','eight-way'});
saveas(gcf,'LRU_picture.png')