# from lsht import lvsh_dynamic, lvsh_recursive
# from time import process_time


# if __name__ == "__main__":
#     str_1a = "re"
#     str_1b = "re"

#     middle = 0
#     lvsh_dynamic(str_1a, str_1b) #Warmup
#     for i in range(1):
#         start = process_time()
#         lvsh_dynamic(str_1a, str_1b)
#         middle = process_time() - start
#     middle /= 1
#     print(f"Dynamic time {middle} for strings '{str_1a}' and '{str_1b}'")

#     middle = 0
#     lvsh_recursive(str_1a, str_1b)
#     for i in range(1):
#         start = process_time()
#         lvsh_recursive(str_1a, str_1b)
#         middle = process_time() - start
#     middle /= 1
#     print(f"Recursive time {middle} for strings '{str_1a}' and '{str_1b}'")


#     str_2a = "reka123123"
#     str_2b = "muka686878"

#     middle = 0
#     lvsh_dynamic(str_2a, str_2b)
#     for i in range(1):
#         start = process_time()
#         lvsh_dynamic(str_2a, str_2b)
#         middle = process_time() - start
#     middle /= 1
#     print(f"Dynamic time {middle} for strings '{str_2a}' and '{str_2b}'")


#     middle = 0
#     lvsh_recursive(str_2a, str_2a)
#     for i in range(1):
#         start = process_time()
#         lvsh_recursive(str_2a, str_2b)
#         middle = process_time() - start
#     middle /= 1
#     print(f"Recursive time {middle} for strings '{str_2a}' and '{str_2b}'")

#     str_3a = "test"
#     str_3b = "testtest"
#     print(f"number for recursive {lvsh_recursive(str_3a, str_3b)} for str1: {str_3a}, str2: {str_3b}")
#     print(f"number for dynamic {lvsh_dynamic(str_3a, str_3b)} for str1: {str_3a}, str2: {str_3b}")

