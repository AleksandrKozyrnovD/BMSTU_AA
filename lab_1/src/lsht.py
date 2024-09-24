def tail(string : str):
    return string[1:]

def head(string : str):
    return string[0]


def lvsh_recursive(string1 : str, string2 : str):
    if len(string1) == 0:
        return len(string2)
    if len(string2) == 0:
        return len(string1)

    if head(string1) == head(string2):
        return lvsh_recursive(tail(string1), tail(string2))
    else:
        a = lvsh_recursive(string1, tail(string2)) + 1
        b = lvsh_recursive(tail(string1), string2) + 1
        c = lvsh_recursive(tail(string1), tail(string2)) + 1

        return min(a, b, c)

    return 0

#assuming i, j != 0 so i > 0 and j > 0
def lvsh_matrix_min(matrix, i, j):
    return min(matrix[i - 1][j], matrix[i][j - 1], matrix[i - 1][j - 1])

def lvsh_dynamic(string1 : str, string2 : str):
    lenstr1 = len(string1)
    lenstr2 = len(string2)
    if lenstr1 == 0:
        return lenstr2
    if lenstr2 == 0:
        return lenstr1
    
    matrix = [[0 for _ in range(lenstr2 + 1)] for _ in range(lenstr1 + 1)]
    for i in range(1, lenstr1 + 1):
        matrix[i][0] = i
    for j in range(1, lenstr2 + 1):
        matrix[0][j] = j

    for i in range(lenstr1):
        for j in range(lenstr2):
            if string1[i] == string2[j]:
                matrix[i + 1][j + 1] = lvsh_matrix_min(matrix, i + 1, j + 1)
            else:
                matrix[i + 1][j + 1] = lvsh_matrix_min(matrix, i + 1, j + 1) + 1

    test = min(lenstr1, lenstr2)
    return matrix[test][test] + abs(lenstr1 - lenstr2)

def dlvsh_dynamic(string1: str, string2: str):
    n = len(string1)
    m = len(string2)


    matrix = [[0 for _ in range(m + 1)] for _ in range(n + 1)]
    for i in range(1, n + 1):
        matrix[i][0] = i
    for j in range(1, m + 1):
        matrix[0][j] = j

    for i in range(1, n + 1):
        for j in range(1, m + 1):
            A = matrix[i - 1][j] + 1    
            B = matrix[i][j - 1] + 1 
            C = matrix[i - 1][j - 1]

            if string1[i - 1] != string2[j - 1]:
                C += 1

            matrix[i][j] = min(A, B, C)

            if i > 1 and j > 1 and string1[i - 1] == string2[j - 2] and string1[i - 2] == string2[j - 1]:
                transpose = matrix[i - 2][j - 2] + 1
                matrix[i][j] = min(matrix[i][j], transpose)

    return matrix[n][m]

if __name__ == "__main__":
    str1 = "testtest"
    str2 = "test"
    print(lvsh_dynamic(str1, str2))
    print(lvsh_recursive(str1, str2))
    print(dlvsh_dynamic(str1, str2))
