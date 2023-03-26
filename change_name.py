import re

carNumber = 1


def replace(file, pattern, subst):
    with open(file, "r+") as f:
        data = f.read()
        data = re.sub(pattern, subst, data)
        f.seek(0)
        f.write(data)
        f.truncate()


replace(
    "lib/BTController/BTController.cpp",
    r"\"Laser Car [\d]+\"",
    '"Laser Car ' + str(carNumber) + '"',
)

replace(
    "lib/BTController/BTController.cpp",
    r"\'L\', \'a\', \'s\', \'e\', \'r\', \' \', \'C\', \'a\', \'r\', \' \', \'[\d]+\'",
    "'L', 'a', 's', 'e', 'r', ' ', 'C', 'a', 'r', ' ', '" + str(carNumber) + "'",
)
