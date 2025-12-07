class Interval():
    def __init__(self, start: float, end: float):
        self.start = start
        self.end = end


class File():
    def __init__(self, path: str):
        self.path = path

    def get_interval(self, start: float | str, end: float | str) -> Interval:
        return Interval(start, end)

    def write_interval(self, interval: Interval): pass


def open(path: str) -> File: pass
def create(path: str) -> File: pass
def exists(path: str) -> bool: pass
