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


#��ȡ�����ո����ӵ�������������split()����
#split()��������һ���б��б��е�Ԫ���ǰ��տո�ָ��
#split()[0]��ʾ��һ��Ԫ�أ��Դ�����

#���ܳ��ּ�Ǯ��ͬ��������ڴ�����£���Ҫ��֮ǰ��value������ӣ��������Ǹ���
#������if in�������Ƿ������ͬ��key��������ڣ����value�������

#�ֵ��е�value��keyĬ��Ϊ�ַ�������Ҫת��Ϊ����
