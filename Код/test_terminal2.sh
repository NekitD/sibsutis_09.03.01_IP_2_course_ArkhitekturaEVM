echo -e "\033[2J"

echo -e "\033(0"

echo -e "\033[10;5Hlqqqqqqk"

for i in {11..17}; do
    echo -e "\033[$i;5Hx      x"
done
echo -e "\033[18;5Hmqqqqqqj"

for i in {11..14}; do
    echo -e "\033[$i;6Ha    a"
done

echo -e "\033[14;7Haaaa"

for i in {14..17}; do
    echo -e "\033[$i;11Ha"
done

echo -e "\033[20;1H"


echo -e "\033[20;5Hlqqqqqqk"

for i in {21..27}; do
    echo -e "\033[$i;5Hx      x"
done
echo -e "\033[28;5Hmqqqqqqj"


echo -e "\033[21;6Haaaaaa"
for i in {22..23}; do
    echo -e "\033[$i;6H    aa"
done
echo -e "\033[24;6Haaaaaa"

for i in {25..26}; do
    echo -e "\033[$i;6Haa"
done
echo -e "\033[27;6Haaaaaa"


#echo -e "\033[20;1H"
#echo -e "\033[20;5Hlqqqqqqk"

#for i in {21..27}; do
#    echo -e "\033[$i;5Hx      x"
#done
#echo -e "\033[28;5Hmqqqqqqj"


#echo -e "\033[16;6Haaaaaa"
#for i in {17..18}; do
#    echo -e "\033[$i;6H    aa"
#done
#echo -e "\033[19;6Haaaaaa"

#for i in {20..21}; do
#    echo -e "\033[$i;6Haa"
#done
#echo -e "\033[22;6Haaaaaa"


# echo -e "\033[20;5Hlqqqqqqqqqqqqqqqk"
# for i in {21..29}; do
#     echo -e "\033[$i;5Hx               x"
# done
# echo -e "\033[30;5Hmqqqqqqqqqqqqqqqj"

# echo -e "\033[10;6H  aa   aaaaaa"
# for i in {11..13}; do
#     echo -e "\033[$i;6H  aa       aa"
# done
# echo -e "\033[14;6H  aa   aaaaaa"

# for i in {15..17}; do
#     echo -e "\033[$i;6H  aa   aa"
# done
# echo -e "\033[18;6H  aa   aaaaaa"

echo -e "\033(B"
