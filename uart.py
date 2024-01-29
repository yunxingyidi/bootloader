def add_header(input_file, output_file):
    with open(input_file, 'rb') as infile:
        original_data = infile.read()

    header1 = bytes([0xff, 0x02, 0xff, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x07, 0x06])

    header2 = bytes([0xff, 0x80, 0x00, 0x20, 0x00, 0x00, 0x00, 0x00, 0x2e])

    new_data = header1 + original_data[:8181] + header2 + original_data[8192:]

    with open(output_file, 'wb') as outfile:
        outfile.write(new_data)

input_file_path = 'rv32ui-p-sw.bin'
output_file_path = 'output.bin'

add_header(input_file_path, output_file_path)
