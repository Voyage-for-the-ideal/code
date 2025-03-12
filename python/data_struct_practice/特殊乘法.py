input_str=input()
total=0
fir_str=input_str.split()[0]
sec_str=input_str.split()[1]
fir_len=len(fir_str)
sec_len=len(sec_str)
for i in range(0,fir_len):
    for j in range(0,sec_len):
       total+=int(fir_str[i])*int(sec_str[j])
print(total) 

#len()函数返回字符串的长度
#字符串的索引从0开始