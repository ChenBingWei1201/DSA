import subprocess
import sys

def run_test(executable, input_file, answer_file):
    # Reading input for the executable
    with open(input_file, 'r') as file:
        input_data = file.read()

    # Running the executable with the input data
    process = subprocess.run(executable, input=input_data, text=True, capture_output=True, shell=True)
    output = process.stdout.strip()

    # Reading the expected answer
    with open(answer_file, 'r') as file:
        expected_output = file.read().strip()

    # Comparing the output with the expected answer
    if output == expected_output:
        return "Test Passed: Output matches the expected answer."
    else:
        return "Test Failed: Output does not match the expected answer.\nExpected: {}\nGot: {}".format(expected_output, output)

def main():
    if len(sys.argv) != 4:
        print("Usage: python3 testbench.py <executable name> <input file> <answer file>")
        sys.exit(1)

    executable = sys.argv[1]
    input_file = sys.argv[2]
    answer_file = sys.argv[3]
    result = run_test(executable, input_file, answer_file)
    print(result)

if __name__ == "__main__":
    main()
