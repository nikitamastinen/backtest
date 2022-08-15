from datetime import datetime

import time
import ujson
from typing import Dict, List, Any
import socket


class Position:
    SHORT = 'S'
    LONG = 'L'
    UNSET = 'N'


class ClosePositionOption:
    def __init__(self, take_profit: float, stop_loss: float, is_fixed: bool = False):
        self.takeProfit = take_profit
        self.stopLoss = stop_loss
        self.isFixed = is_fixed

    def dict(self):
        return dict({
            "t": self.takeProfit,
            "s": self.stopLoss,
            "f": self.isFixed,
        })


class StrategyEvent:
    def __init__(
            self,
            timestamp: int,
            position: str,
            close_position_options: List[ClosePositionOption],
            is_close_long: bool = False,
            is_close_short: bool = False,
    ):
        self.timestamp = timestamp
        self.isCloseLong = is_close_long
        self.isCloseShort = is_close_short
        self.position = position
        self.close_position_options = close_position_options

    def dict(self):
        return dict({
            "t": self.timestamp,
            "cl": self.isCloseLong,
            "cs": self.isCloseShort,
            "p": self.position,
            "o": [i.dict() for i in self.close_position_options]
        })


class Trade:
    def __init__(self,
                 trade: Dict[str, Any]):
        self.openTime = trade["o"]
        self.closeTime = trade["c"]
        self.position = trade["p"]
        self.fundBefore = trade["fb"]
        self.fundAfter = trade["fa"]
        self.reward = trade["r"]

    def __str__(self):
        return f"open: {datetime.utcfromtimestamp(self.openTime).strftime('%d.%m.%Y %H:%M:%S')}\n" + \
               f"close: {datetime.utcfromtimestamp(self.closeTime).strftime('%d.%m.%Y %H:%M:%S')}\n" + \
               f"position: {self.position}\n" + \
               f"reward: {self.reward}\n"


def test(swap_path: str, coin: str, strategy: List[StrategyEvent], host="127.0.0.1", port=8000, start=0) -> List:
    serialized_strategy = {"coin": coin, "strategy": [i.dict() for i in strategy]}
    strategy_path = swap_path + f"\\{coin}_{int(time.time())}.json"

    with open(strategy_path, "w+") as out:
        ujson.dump(serialized_strategy, out)

    with socket.socket(socket.AF_INET, socket.SOCK_STREAM) as s:
        s.connect((host, port))
        s.sendall(strategy_path.encode())

        metafile_path = s.recv(1024)

        with open(metafile_path, "r+") as inp:
            result = ujson.load(inp)

        return [Trade(i) for i in result]
