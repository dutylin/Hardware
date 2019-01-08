clear all;
X = load('gauss_201812271.txt');

manetic_original=[X(:,1:3)];
manetic_filter=[X(:,4:6)];
manetic_mpv=[X(:,7:9)];
mpv_ti=[X(:,10:12)];
te=[X(:,13:15)];
error1=[X(:,16:18)];
error2=[X(:,19:21)];

len=size(te);
j=1;
mpv_ti2=zeros(len(1),3);
manetic_mpv2=zeros(len(1),3);
for k=1:1:3
    for i=1:1:(len(1)-2)
        if mpv_ti(i,k)~= mpv_ti(i+1,k)
            mpv_ti2(j,k)=mpv_ti(i,k);
            manetic_mpv2(j,k)=manetic_mpv(i,k);
            j=j+1;
        end
    end
end
   j=0;
    for i=1:1:(len(1)-2)
        if mpv_ti2(i,3)== mpv_ti2(i+1,3)
            break;
            
        else
             j=j+1;
        end
    end

len=size(manetic_original);
x=0:1/9.58:(len(1)-1)/9.58;

magnetic_X=manetic_original(:,1);
magnetic_Y=manetic_original(:,2);
magnetic_Z=manetic_original(:,3);

magnetic_filter_X=manetic_filter(:,1);
magnetic_filter_Y=manetic_filter(:,2);
magnetic_filter_Z=manetic_filter(:,3);

manetic_mpv=[X(:,7:9)];
mpv_ti=[X(:,10:12)];
te=[X(:,13:15)];
error1=[X(:,16:18)];
error2=[X(:,19:21)];


subplot(3,1,1);
plot(te(:,1),error1(:,1),'b',te(:,1),error1(:,2),'r',te(:,1),error1(:,3),'k','LineWidth',2);
legend('Magne-X-E','Magne-Y-E','Magne-Z-E');
grid on;
xlabel('time(second)');
ylabel('error');

subplot(3,1,2);
plot(mpv_ti2(1:j,1),manetic_mpv2(1:j,1),'b',mpv_ti2(1:j,1),manetic_mpv2(1:j,2),'r',mpv_ti2(1:j,1),manetic_mpv2(1:j,3),'k','LineWidth',2);
legend('Magne-X-MPV','Magne-Y-MPV','Magne-Z-MPV');
grid on;
xlabel('time(second)');
ylabel('gauss');


subplot(3,1,3);
plot(te(:,1),magnetic_X,'b--',te(:,1),magnetic_Y,'r--',te(:,1),magnetic_Z,'k--',te(:,1),magnetic_filter_X,'b',te(:,1),magnetic_filter_Y,'r',te(:,1),magnetic_filter_Z,'k','LineWidth',3);
legend('Magne-X-origin','Magne-Y-origin','Magne-Z-origin','Magne-X-base','Magne-Y-base','Magne-Z-base');
grid on;
xlabel('time(second)');
ylabel('gauss');













subplot(3,1,1);
plot(x,error1(:,1),'b',x,error1(:,2),'r',x,error1(:,3),'k','LineWidth',2);
legend('Magne-X-E','Magne-Y-E','Magne-Z-E');
grid on;
xlabel('time(second)');
ylabel('error');
subplot(3,1,2);
%plot(x,manetic_mpv(:,1),'b--',x,manetic_mpv(:,2),'r--',x,manetic_mpv(:,3),'k--',x,magnetic_X,'b',x,magnetic_Y,'r',x,magnetic_Z,'k','LineWidth',2);

plot(x,manetic_mpv(:,1),'b',x,manetic_mpv(:,2),'r',x,manetic_mpv(:,3),'k','LineWidth',2);
legend('Magne-X-MPV','Magne-Y-MPV','Magne-Z-MPV');
grid on;
xlabel('time(second)');
ylabel('gauss');

subplot(3,1,3);
plot(x,magnetic_X,'b--',x,magnetic_Y,'r--',x,magnetic_Z,'k--',x,magnetic_filter_X,'b',x,magnetic_filter_Y,'r',x,magnetic_filter_Z,'k','LineWidth',3);
legend('Magne-X-origin','Magne-Y-origin','Magne-Z-origin','Magne-X-base','Magne-Y-base','Magne-Z-base');
grid on;
xlabel('time(second)');
ylabel('gauss');