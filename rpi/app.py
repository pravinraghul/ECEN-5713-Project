# Custom diagnostic test script
# 
# Author: Pravin Raghul S
devfile = "/dev/gpiobit"

# List of the test cases and their descriptions.
TESTS = {
    "1" : "On-board LED Test",
    "2" : "Serial Output Test"
}

def print_test_info():
    """ Prints the list of tests available
    """
    print("=================")
    print("Diagnostic Tests:")
    for id, desc in TESTS.items():
        print(f'{id} - {desc}')
    print("q - Quit")

def write_to_device(device_path):
    """ Write the valid data to the device
    """
    try:
        user_input = input("Enter the Test ID: ")
        
        if user_input == 'q':
            return 1
        
        if user_input not in TESTS.keys():
            print ("Invalid input")
            return 0

        with open(device_path, "w") as device_file:
            device_file.write(user_input)
            print(f"Data '{user_input}' successfully written to {device_path}")
            return 0

    except IOError as e:
        print(f"Error writing to device: {e}")
        return -1

def main():
    while True:
        print_test_info()
        ret = write_to_device(devfile)
        if ret != 0:
            break

if __name__ == "__main__":
    main()
