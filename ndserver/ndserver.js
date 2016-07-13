var net = require('net');
var chatServer = net.createServer(),
    clientList = [];
var reciv_obj ={};

chatServer.on('connection', function(client) {
    console.log("new connection");
    client.name = client.remoteAddress + ':' + client.remotePort;
    client.write('Hi ' + client.name + '!\n');

    clientList.push(client);

    client.on('data', function(data) {
         reciv_obj  = JSON.parse(data);
        console.log(reciv_obj.usrname);
        client.name = reciv_obj.usrname;
        if(reciv_obj.msg==''){
            console.log("usr: "+reciv_obj.usrname+" login");
        }else{
            console.log(reciv_obj.usrname+"says: "+reciv_obj.msg);
        }
        broadcast(data, client);
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
            clientList[i].write(client.name + " says " + message);
        }
    }
}

chatServer.listen(9999);