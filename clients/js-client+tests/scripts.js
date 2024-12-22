document.addEventListener('DOMContentLoaded',function(){
    // Получаем элементы
    const selectOption = document.getElementById('select_option');
    const inputGroup = document.getElementById('input_group');
    const inputGroup2 = document.getElementById('input_group2');

    // Функция для отображения нужного блока в зависимости от выбранного значения
    selectOption.addEventListener('change', function () {
        if (selectOption.value === 'Find') {
            inputGroup.style.display = 'block';
            inputGroup2.style.display = 'none';
        } else if (selectOption.value === 'Update') {
            inputGroup.style.display = 'none';
            inputGroup2.style.display = 'block';
        }
    });

    // Вызов функции сразу для начальной настройки
    selectOption.dispatchEvent(new Event('change'));

});

document.addEventListener('DOMContentLoaded', function() {
    const sendButton = document.getElementById('send_button');

    // Один обработчик для кнопки
    sendButton.addEventListener('click', function() {
        console.log('Button clicked!');

        const selectOption = document.getElementById('select_option').value;
        let message = '';

        if (selectOption === 'Find') {
            const word = document.getElementById('input1').value;
            const lowerCaseWord = word.toLowerCase(); // Преобразуем слово в нижний регистр
            message = `find ${lowerCaseWord}`;
        } else if (selectOption === 'Update') {
            const num1 = document.getElementById('input2').value;
            const num2 = document.getElementById('input3').value;
            message = `update ${num1} ${num2} C:\\Users\\RT\\Desktop\\7semestr\\aclImdb\\test\\neg`;
        }

        // Отправка запроса на сервер
        fetch('http://localhost:3000', {
            method: 'POST',
            body: message
        })
        .then(response => response.text())
        .then(data => {
            document.getElementById('anwer_container').textContent = data;
        })
        .catch(error => {
            console.error('Error:', error);
        });
    });
});
