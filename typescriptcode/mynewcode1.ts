class student{
    fullname:string;
    constructor(public firstname,public middleinitial,public lastname){
        this.fullname=firstname+" "+middleinitial+" "+lastname;
    }
}

interface Person{
    firstname:string;
    lastname:string;
}
function greeter(person:student){
    return "hello,"+person.fullname;
}
var user=new student("li","run","xin");
document.body.innerHTML=greeter(user);