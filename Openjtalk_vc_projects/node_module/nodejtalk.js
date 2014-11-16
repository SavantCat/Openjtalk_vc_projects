var nodejtalk   = require('../Debug/nodejtalk');
var fs = require("fs");

var file_name = "voice.wav"
var text = 'あらあら、これはこれは。犬の遺体が捨てられていると思ったら,,,なんだ、あららぎくんじゃないの？';

nodejtalk.setup("G:\\github\\Openjtalk_vc_projects\\Openjtalk_vc_projects\\Debug\\dic",
            "G:\\github\\Openjtalk_vc_projects\\Openjtalk_vc_projects\\Debug\\voice\\mei_happy.htsvoice",
            48000,
            file_name);

nodejtalk.run(text);

nodejtalk.end();

var buffer = fs.readFileSync( file_name );
console.log("Buffer size:"+buffer.length);
/*
var sys = require('sys');
for(var i=0;i<buffer.length;i++){
    if (i % 15 == 0 && i>0) {
        console.log("");
    }
    sys.print(buffer[i]);
}
*/

