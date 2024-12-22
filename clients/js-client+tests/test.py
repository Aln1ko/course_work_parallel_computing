import socket
import threading
import time

SERVER_HOST = '127.0.0.1'  # Адреса вашого сервера
SERVER_PORT = 8080         # Порт вашого сервера
NUM_REQUESTS = 1000        # Кількість запитів для тестування
NUM_THREADS = 50           # Кількість одночасних потоків

# Функція для підключення до сервера та відправлення запиту
def send_request(request_id):
    try:
        with socket.socket(socket.AF_INET, socket.SOCK_STREAM) as s:
            s.connect((SERVER_HOST, SERVER_PORT))
            message = "find table"
            s.sendall(message.encode('utf-8'))
            response = s.recv(1024)
            print(f"Response to {request_id}: {response.decode('utf-8')}")
    except Exception as e:
        print(f"Error for request {request_id}: {e}")

# Функція для запуску кількох потоків
def load_test():
    threads = []
    for i in range(NUM_REQUESTS):
        t = threading.Thread(target=send_request, args=(i,))
        threads.append(t)
        t.start()

        if len(threads) >= NUM_THREADS:
            for t in threads:
                t.join()
            threads = []

    # Дочекаємось завершення всіх потоків
    for t in threads:
        t.join()

if __name__ == "__main__":
    start_time = time.time()
    load_test()
    end_time = time.time()
    print(f"Completed {NUM_REQUESTS} requests in {end_time - start_time:.2f} seconds")
