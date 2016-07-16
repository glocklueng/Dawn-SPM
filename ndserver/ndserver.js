var net = require('net');
var chatServer = net.createServer(),
    clientList = [];
var reciv_obj ={};
var send_obj={};

chatServer.on('connection', function(client) {
    console.log("new connection from "+client.remoteAddress+":"+client.remotePort);
    client.name = client.remoteAddress + ':' + client.remotePort;
    client.write('Welcome friend form ' + client.remoteAddress+":"+client.remotePort+'!\n');

    clientList.push(client);

    client.on('data', function(data) {
         reciv_obj  = JSON.parse(data);
        client.name = reciv_obj.usrname;
        if('msg' in reciv_obj){
            console.log(reciv_obj.usrname+" says: "+reciv_obj.msg);
            broadcast(reciv_obj.msg, client);
        }else if('file' in reciv_obj){
            send_obj.file = reciv_obj.file;
            broadcast(JSON.stringify(send_obj),client);
        }else{
            broadcast(data,client);
            console.log("usr: "+reciv_obj.usrname+" login");
        }
    });

    client.on('end', function() {
        clientList.splice(clientList.indexOf(client), 1);
    });
    client.on('error', function(e) {
        console.log(e);
    });
});
function broadcast(message, client) {
    for(var i=0;i<clientList.length;i+=1) {
        if(client !== clientList[i]) {
            clientList[i].write(client.name + " says :\n" + message);
        }
    }
}

chatServer.listen(9999);