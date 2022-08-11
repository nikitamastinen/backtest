import socket
import time

from backtest import *


def example():
    start = time.time()
    simple_strategy = [
        StrategyEvent(
            timestamp=1643679299 + i * 10000,
            is_close_long=False,
            is_close_short=False,
            position='L',
            close_position_options=[
                ClosePositionOption(take_profit=1.06, stop_loss=0.98, is_fixed=False),
                ClosePositionOption(take_profit=1000, stop_loss=0.99, is_fixed=False)
            ]
        ) for i in range(100000)
    ]

    print("created candles", time.time() - start)
    start = time.time()

    res = test(
        swap_path="D:\\business\\hackathon\\miap\\backend\\pythonProject\\swap",
        coin="BTC",
        strategy=simple_strategy,
        start=start
    )

    print()

    for i in res:
        print(i)


example()
