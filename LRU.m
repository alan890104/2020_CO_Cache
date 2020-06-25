clear;clc;close all;
A = readmatrix("answer.txt");
col = length(A(1,:));
row = length(A(:,1));
tmp_arr = [];
for i = 1:row
   hold on;
   plot((1:col),A(i,1:col),'-o','linewidth',3); 
end
set(gcf, 'Position',  [500 400 1000 400]);
legend('1K','2K','4K','8K','16K','32K','64K'); 
axis([0 5 0 15]);
xlabel('associativity');  
ylabel('miss rate')  
set(gca,'Xtick',1:4);
set(gca,'Ytick',0:5:20)
grid on;
ytickformat('%d %%')
set(gca,'XtickLabel',{'one-way','two-way','four-way','eight-way'});
saveas(gcf,'LRU_picture.png')