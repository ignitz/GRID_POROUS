%mcc -m a_fixed.m

t = cputime;
A = spconvert(load('outputFullMatrix.txt'));
b = load('outputB.txt');
0

[L,U] = ilu(A,struct('type','ilutp','droptol',1e-4));
[x,flag,rr,it,rv] = bicgstab(A,b,1e-9,1000*length(b),L,U);

rr;
fid = fopen('result.txt','w');
fprintf(fid,'%6.2f \r\n',flag);
fprintf(fid,'%6.2f \r\n','');
fprintf(fid,'%6.10f \r\n',x);
e = cputime-t
fclose('all');
