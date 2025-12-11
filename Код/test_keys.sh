#!/bin/bash

echo "Нажмите клавишу (ввод завершится по ESC):"

while true; do
    read -rsn1 key 
    
    if [[ "$key" == $'\e' ]]; then
        read -rsn1 -t 0.1 tmp
        
        if [[ "$tmp" == "[" ]]; then
            read -rsn3 -t 0.1 tmp
            key+="[$tmp"
            
            case "$key" in
                $'\E[A')    echo -e "\nВы нажали: Стрелка вверх" ;;
                $'\E[B')    echo -e "\nВы нажали: Стрелка вниз" ;;
                $'\E[C')    echo -e "\nВы нажали: Стрелка вправо" ;;
                $'\E[D')    echo -e "\nВы нажали: Стрелка влево" ;;
                $'\E[15~') echo -e "\nВы нажали: F5" ;;
                $'\E[17~') echo -e "\nВы нажали: F6" ;;
                *)          echo -e "\nНеизвестная ESC последовательность: $(echo -n "$key" | xxd -ps)" ;;
            esac
        else
            echo -e "\nВы нажали: ESC"
            break
        fi
    else
        case "$key" in
            $'\n') echo -e "\nВы нажали: Enter" ;;
            [a-zA-Z0-9+-]) echo -e "\nВы нажали: $key" ;;
            *) echo -e "\nНеподдерживаемая клавиша: $(echo -n "$key" | xxd -ps)" ;;
        esac
    fi
done