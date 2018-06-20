%kroki1 = [-500 2000 0 -300 200];
%kroki2 = [-500 2000 0 -300 200];
%kroki3 = [-500 2000 0 -300 200];
% %% kolko
% t = 0:0.1:2*pi;
% traj1 = floor(1000*sin(t));
% traj2 = floor(1000*sin(t+2*pi/3));
% traj3 = floor(1000*sin(t+4*pi/3));
%
% kroki1=traj1(2:end)-traj1(1:end-1);
% kroki2=traj2(2:end)-traj2(1:end-1);
% kroki3=traj3(2:end)-traj3(1:end-1);
%
% for (i=1:length(kroki1))
% fprintf(s, 'x %d y %d z %d \n',[kroki1(i) kroki2(i) kroki3(i)]);
% while(s.BytesAvailable == 0)    ;
% end
% while(s.BytesAvailable > 0)
%     fscanf(s)
% end
% end
%% Rysujemy drukarke
figure(3)
temp_zadana = 70;
x1 = 0;
y1= 0;
x2 = 0;
y2 = 28;
x3 = 24;
y3 = 14;
kol1 = line([x1 x1],[y1 y1],[0 60]);
kol2 = line([x2 x2], [y2 y2], [0 60]);
kol3 = line([x3 x3], [y3 y3], [0 60]);
view(-19,36);
axis([-10 50 -10 50 0 60]);
axis square;
xc = 12;
yc = 14;
zc = 30;
r = 20;
z_sil_1 = zc + sqrt(r^2 - (x1 - xc)^2 - (y1 - yc)^2);
z_sil_2 = zc + sqrt(r^2 - (x2 - xc)^2 - (y2 - yc)^2);
z_sil_3 = zc + sqrt(r^2 - (x3 - xc)^2 - (y3 - yc)^2);
ramie_1 = line([xc, x1],[yc, y1],[zc,z_sil_1]);
ramie_2 = line([xc, x2],[yc, y2],[zc,z_sil_2]);
ramie_3 = line([xc, x3],[yc, y3],[zc,z_sil_3]);
% przykladowa animacja
for t= 0:0.01:10
    xc = 12 + cos(t); %+sin(t)
    yc = 14 + sin(t); %+cos(t)
    zc = 30;
    z_sil_1 = zc + sqrt(r^2 - (x1 - xc)^2 - (y1 - yc)^2);
    z_sil_2 = zc + sqrt(r^2 - (x2 - xc)^2 - (y2 - yc)^2);
    z_sil_3 = zc + sqrt(r^2 - (x3 - xc)^2 - (y3 - yc)^2);
    ramie_1.XData = [xc x1];
    ramie_1.YData = [yc y1];
    ramie_1.ZData = [zc z_sil_1];
    ramie_2.XData = [xc x2];
    ramie_2.YData = [yc y2];
    ramie_2.ZData = [zc z_sil_2];
    ramie_3.XData = [xc x3];
    ramie_3.YData = [yc y3];
    ramie_3.ZData = [zc z_sil_3];
    drawnow;
    
    if(t>0)
        krok1=round(200*(z_sil_1 - stare1));
        krok2=round(200*(z_sil_2 - stare2));
        krok3=round(200*(z_sil_3 - stare3));
        %for (i=1:length(krok1))
        fprintf(s, 'x %d y %d z %d temp %d\n',[krok1 krok2 krok3 temp_zadana]);
        while(s.BytesAvailable == 0);
        end
        while(s.BytesAvailable > 0)
            fscanf(s)
        end
    end
    
    stare1=z_sil_1;
    stare2=z_sil_2;
    stare3=z_sil_3;
end

