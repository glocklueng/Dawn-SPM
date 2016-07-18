var net = require('net');
var chatServer = net.createServer(),
    clientList = [];
var reciv_obj ={};

chatServer.on('connection', function(client) {
    console.log("new connection from "+client.remoteAddress+":"+client.remotePort);
    client.name = client.remoteAddress + ':' + client.remotePort;
    client.write('Welcome friend form ' + client.remoteAddress+":"+client.remotePort+'!\n');

    clientList.push(client);

    client.on('data', function(data) {
         reciv_obj  =  JSON.parse(data);
        client.name = reciv_obj.usrname;
        if(reciv_obj.type=="msg"){
            broadcast(reciv_obj,client);
            console.log(reciv_obj.usrname+" says: "+reciv_obj.content);
            console.log(reciv_obj.toname)
        }else if(reciv_obj.type =="file"){
            broadcast(reciv_obj,client);
            console.log(reciv_obj.usrname+"send a file");
            console.log(reciv_obj);
        }else if(reciv_obj.type == "login"){
            refresh(client);
        }else{
            broadcast(reciv_obj,client);
            console.log(data);
            console.log("usr: send a data"+data);
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
            console.log(client.name);
            console.log(message);
            clientList[i].write(JSON.stringify(message));
        }
    }
}
function refresh(client) {
    reciv_obj.toname ="all";
    reciv_obj.type = "refresh";
    for(var i=0;i<clientList.length;i+=1) {
            reciv_obj.content =  clientList[i].name;
            console.log(client.name);
            console.log(message);
            clientList[i].write(JSON.stringify(reciv_obj));

    }
}

chatServer.listen(9999);