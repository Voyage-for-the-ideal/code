def fee(total,farmer):
    farmer_list = {}
    price = 0
    amount=0
    total_price=0
    for _ in range(1,farmer+1):
        input_str = input()
        price=int(input_str.split()[0])
        amount=int(input_str.split()[1])
        if price in farmer_list:
            farmer_list[price]=(farmer_list[price])+(amount)
        else:
            farmer_list[price]=amount
    farmer_list = dict(sorted(farmer_list.items(), key=lambda x: x[0]))
    for key in farmer_list:
        if((farmer_list[key])<=total):
            total-=(farmer_list[key])
            total_price+=(key)*(farmer_list[key])
        else:
            total_price+=(key)*(total)
            break
    return total_price
total,farmer = map(int, input().split())
print(fee(total,farmer))


#读取两个空格连接的整数，可以用split()函数
#split()函数返回一个列表，列表中的元素是按照空格分割的
#split()[0]表示第一个元素，以此类推

#可能出现价钱相同的情况，在此情况下，需要对之前的value进行相加，而不是是覆盖
#可以用if in语句检验是否存在相同的key，如果存在，则对value进行相加

#字典中的value和key默认为字符串，需要转换为整数
