var info = {
    udn : 1001,
    utype : 1,
    remote_udn : 9001,
    remote_utype : 2
}
console.log(info["udn"])

info.tid = 22;
console.log(info);

info["duplex"] = "full";
console.log(info);

