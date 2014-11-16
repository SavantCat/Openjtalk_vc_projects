var a,b,c,d,e;

a = 'abcdef';
b = new Buffer(a, 'ascii');
c = b.toString('base64');
d = new Buffer(c, 'base64');
e = d.toString('ascii');
console.log('a='+a);
console.log('c='+c);
console.log('e='+e);

a = '日本語表示';
b = new Buffer(a, 'utf8');
c = b.toString('base64');
d = new Buffer(c, 'base64');
e = d.toString('utf8');
console.log('a='+a);
console.log('c='+c);
console.log('e='+e);