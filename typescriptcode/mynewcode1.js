var student = (function () {
    function student(firstname, middleinitial, lastname) {
        this.firstname = firstname;
        this.middleinitial = middleinitial;
        this.lastname = lastname;
        this.fullname = firstname + " " + middleinitial + " " + lastname;
    }
    return student;
}());
function greeter(person) {
    return "hello," + person.fullname;
}
var user = new student("li", "run", "xin");
document.body.innerHTML = greeter(user);
