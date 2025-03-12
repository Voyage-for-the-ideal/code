let height = parseFloat(prompt('请输入身高(m):'));
let weight = parseFloat(prompt('请输入体重(kg):'));

// TODO:
let bmi = weight/(height*height);
if(bmi<18.5){
console.log("guoqing");
}
else if(bmi<=25){
console.log("zhengchang");
}
else if(bmi<=28){
console.log("guozhong");}
else if(bmi<=32){
console.log("feipang");}
else if(bmi>32){
console.log("yanzhongfeipang");}
