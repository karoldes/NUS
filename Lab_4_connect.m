%% Polaczenie z zabezpieczeniem
if ~exist('s','var')    
s = serial('/dev/ttyUSB0');
s.BaudRate = 57600;
end
if s.status(1) == 'c';
    fopen(s);
end
