clear all;
X = load('gauss.txt');
manetic_original=[X(:,1:3)];
manetic_filter=[X(:,4:6)];
len=size(manetic_original);
 x=0:1/10:(len(1)-1)/10;
 magnetic_X=manetic_original(:,1);
 magnetic_Y=manetic_original(:,2);
 magnetic_Z=manetic_original(:,3);
 
  magnetic_filter_X=manetic_filter(:,1);
 magnetic_filter_Y=manetic_filter(:,2);
 magnetic_filter_Z=manetic_filter(:,3);
 
 plot(x,magnetic_X,x,magnetic_Y,x,magnetic_Z,x,magnetic_filter_X,x,magnetic_filter_Y,x,magnetic_filter_Z,'LineWidth',2);
 legend('Magne-X-origin','Magne-Y-origin','Magne-Z-origin','Magne-X-base','Magne-Y-base','Magne-Z-base');
 grid on;
 xlabel('time(second)');
  ylabel('gauss');