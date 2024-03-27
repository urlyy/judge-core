'''
# 自定义judge函数  
def judge(lines):  
    for line in lines:
        i = int(line)  
        if i % 2 == 1:  
            return False  
    return True  
'''


'''
下面这些都是模板,上面的是出题人自定义的,上面的函数名必须为judge,参数是list[str],必须返回True或False
'''
import signal  
import sys  
from contextlib import contextmanager  
  
@contextmanager  
def time_limit(seconds):  
    def signal_handler(signum, frame):  
        raise Exception()  
    signal.signal(signal.SIGALRM, signal_handler)  
    signal.alarm(seconds)  
    try:  
        yield  
    finally:  
        signal.alarm(0)
        

try:  
    with open(sys.argv[1], 'r') as file:  
        lines = file.readlines()  
    with time_limit(sys.argv[2]):
        res = judge(lines)  
except Exception as e:  
        res = False
with open(sys.argv[3], 'w') as f:  
    f.write(res)