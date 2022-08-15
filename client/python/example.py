from backtest import *


def example():
    simple_strategy = [
        StrategyEvent(
            timestamp=1643679300 + i * 10000,
            is_close_long=False,
            is_close_short=False,
            position='L',
            close_position_options=[
                ClosePositionOption(take_profit=1.06, stop_loss=0.95, is_fixed=False),
                ClosePositionOption(take_profit=1000, stop_loss=0.99, is_fixed=False)
            ]
        ) for i in range(100000)
    ]

    res = test(
        swap_path="D:\\business\\hackathon\\miap\\backend\\pythonProject\\swap",
        coin="BTC",
        strategy=simple_strategy,
    )

    print()
    for i in res:
        print(i)


example()
