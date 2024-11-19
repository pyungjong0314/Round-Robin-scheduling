# Round-Robin-scheduling
C 언어와 POSIX Thread(pthread)를 활용하여 Round Robin 방식으로 프로세스 스케줄링을 구현하였습니다.

## 주요 기능
- **Queue 관리:** 실행 준비, 대기, 종료 상태의 Thread를 각각 다른 Queue에서 관리.
- **Thread Control Block (TCB):** Thread 정보를 저장하는 구조체를 사용하여 상태를 효율적으로 추적.
- **Round Robin Scheduling:** 정해진 시간 할당량(Time Quantum) 동안 Ready Queue에서 Thread를 실행.

## Queue
Thread의 상태에 따라 아래 3가지 Queue로 나누어 관리합니다:
- **Ready Queue:** 실행 준비가 완료된 Thread를 관리.
- **Wait Queue:** 실행 중 대기 상태로 전환된 Thread를 관리.
- **Zombie Queue:** 실행이 종료된 Thread를 관리.

## TCB (Thread Control Block)
Thread의 정보를 저장하는 구조체로, 아래와 같은 정보를 포함합니다:
- **Thread 상태:** Ready, Wait, Zombie 상태.
- **Thread 식별자:** Parent Thread Id, Thread Id.
- **실행 함수:** Thread가 실행할 함수를 저장.
    
## Round Robin 구현
Round Robin 방식은 다음과 같이 작동합니다:
1. **Ready Queue**의 가장 앞에 있는 Thread 실행.
2. **pthread_mutex**와 **pthread_cond**를 활용하여 한 번에 하나의 Thread만 실행.
3. Wait 조건 발생 시, 해당 Thread를 **Wait Queue**로 이동.
4. Wait 조건이 해제되면, 해당 Thread를 **Ready Queue**의 끝으로 이동.
