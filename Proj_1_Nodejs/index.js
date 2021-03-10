const DEFAULT_PROMPT = "308jsh>";

const spawnSync = require("child_process").spawnSync;
const prompt = require('prompt-sync')({sigint: true});

function main(){
    var usrPrompt = getPrompt(process.argv);
    var status = 1;
    do{
        var usrInput = prompt(`${usrPrompt} `);
        var usrInputObj = parseInput(usrInput);

        if(usrInputObj.command === "exit"){
            status = 0;
        }
        else if(usrInputObj.command === "pid"){
            console.log(`Shell PID: ${process.pid}`);
        }
        else if(usrInputObj.command === "ppid"){
            console.log(`Shell's parent ID: ${process.ppid}`);
        }
        else if(usrInputObj.command === "pwd"){
            console.log(process.cwd());
        }
        else if(usrInputObj.command === "cd"){
            changeDirectory(usrInputObj);
        }
        else{
            executeNonBuiltin(usrInputObj.usrInput);
        }
    }while(status);
}

function getPrompt(argArr){
    var pFlag = argArr.indexOf("-p");
    if(pFlag != -1){
        return argArr[pFlag+1];
    }
    else{
        return DEFAULT_PROMPT;
    }
}

function parseInput(usrInput){
    var usrInputObj={};
    usrInputObj.usrInput = usrInput;
    usrInputObj.inputArr = usrInput.match(/\S+/g);
    usrInputObj.isBackground = (usrInput.indexOf("&")!= -1);
    usrInputObj.command = usrInputObj.inputArr[0];
    return usrInputObj;
}

function executeNonBuiltin(commandString){
    try{
        var spawnSyncObj = spawnSync(commandString);
        console.log(`[${spawnSyncObj.pid}] ${commandString}\n`);
        console.log(`${spawnSyncObj.stdout}`);
        console.log(`[${spawnSyncObj.pid}] ${commandString} Exit ${spawnSyncObj.status}`);
    }
    catch(error){
        console.error(error);
    }
}
function changeDirectory(usrInputObj){
    try{
        if(!usrInputObj.inputArr[1]){
            process.chdir(process.env.HOME);
        }
        else{
            process.chdir(usrInputObj.inputArr[1]);
        }
    }
    catch(error){
        console.error(error);
    }
}

main();