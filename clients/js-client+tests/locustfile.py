from locust import User, TaskSet, task, between, events
import socket
import time

class TcpClient:
    def __init__(self, host, port):
        self.host = host
        self.port = port
        self.client_socket = None

    def connect(self):
        if not self.client_socket:
            self.client_socket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
            self.client_socket.connect((self.host, self.port))

    def send(self, message):
        start_time = time.time()

        try:
            self.connect()
            self.client_socket.sendall(message.encode('utf-8'))
            response = self.client_socket.recv(1024)
            response_text = response.decode('utf-8')
            total_time = (time.time() - start_time) * 1000  # время в миллисекундах

            # Успешное выполнение запроса
            events.request.fire(
                request_type="TCP",
                name="send_request",
                response_time=total_time,
                response_length=len(response_text),
                response=response_text,
                exception=None
            )
            return response_text
        except Exception as e:
            total_time = (time.time() - start_time) * 1000

            # Ошибка при выполнении запроса
            events.request.fire(
                request_type="TCP",
                name="send_request",
                response_time=total_time,
                response_length=0,
                response=None,
                exception=e
            )
            return str(e)


    def close(self):
        if self.client_socket:
            self.client_socket.close()
            self.client_socket = None

class TcpTaskSet(TaskSet):
    def on_start(self):
        # Ініціалізація TCP-клієнта при старті
        self.tcp_client = TcpClient("127.0.0.1", 8080)  # Вкажіть адресу та порт вашого сервера

    @task
    def send_request(self):
        message = "find table"
        # message = "table"
        response = self.tcp_client.send(message)
        # print(f"Sent: {message}, Received: {response}")
        # message = "quit"
        # response = self.tcp_client.send(message)

    def on_stop(self):
        # Закриття з'єднання після завершення
        self.tcp_client.close()

class TcpUser(User):
    tasks = [TcpTaskSet]
    wait_time = between(1,1)  # Час очікування між завданнями (1-3 секунди)

# from locust import User, TaskSet, task, between, events
# import socket
# import time

# class TcpClient:
#     def __init__(self, host, port):
#         self.host = host
#         self.port = port

#     def send(self, message):
#         start_time = time.time()

#         try:
#             # Подключение к серверу
#             client_socket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
#             client_socket.connect((self.host, self.port))

#             # Отправка сообщения
#             client_socket.sendall(message.encode('utf-8'))
#             response = client_socket.recv(1024)
#             response_text = response.decode('utf-8')

#             # Вычисление времени выполнения
#             total_time = (time.time() - start_time) * 1000  # время в миллисекундах

#             # Успешное выполнение запроса
#             events.request.fire(
#                 request_type="TCP",
#                 name="send_request",
#                 response_time=total_time,
#                 response_length=len(response_text),
#                 response=response_text,
#                 exception=None
#             )

#             # Закрытие соединения
#             client_socket.close()

#             return response_text
#         except Exception as e:
#             total_time = (time.time() - start_time) * 1000

#             # Ошибка при выполнении запроса
#             events.request.fire(
#                 request_type="TCP",
#                 name="send_request",
#                 response_time=total_time,
#                 response_length=0,
#                 response=None,
#                 exception=e
#             )
#             return str(e)


# class TcpTaskSet(TaskSet):
#     @task
#     def send_request(self):
#         # Отправка запроса с подключением и отключением
#         message = "find table"
#         tcp_client = TcpClient("127.0.0.1", 8080)  # Подключение к серверу
#         response = tcp_client.send(message)  # Отправка и получение ответа
#         # print(f"Sent: {message}, Received: {response}")


# class TcpUser(User):
#     tasks = [TcpTaskSet]
#     wait_time = between(0.1, 0.1)  # Время ожидания между запросами
