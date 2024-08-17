# Simple Test application
devfile = "/dev/gpiobit"

def print_test_info():
    print("Test informations")
    print("1- On-board LED Test")
    print("2- Serial print Test")

def write_to_device(device_path):
    try:
        user_input = input("Enter the Test ID:")
        # Open the device file in write mode
        with open(device_path, "w") as device_file:
            # Write user input to the device file
            device_file.write(int(user_input))
            print(f"Data '{user_input}' successfully written to {device_path}")

    except IOError as e:
        print(f"Error writing to device: {e}")

if __name__ == "__main__":
    print_test_info()
    write_to_device(devfile)
