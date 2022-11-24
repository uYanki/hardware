def createWavHeader(sampleRate, bitsPerSample, num_channels, datasize):
    # (4byte) Marks file as RIFF
    o = bytes("RIFF", 'ascii')
    # (4byte) File size in bytes excluding this and RIFF marker
    o += (datasize + 36).to_bytes(4, 'little')
    # (4byte) File type
    o += bytes("WAVE", 'ascii')
    # (4byte) Format Chunk Marker
    o += bytes("fmt ", 'ascii')
    # (4byte) Length of above format data
    o += (16).to_bytes(4, 'little')
    # (2byte) Format type (1 - PCM)
    o += (1).to_bytes(2, 'little')
    # (2byte)
    o += (num_channels).to_bytes(2, 'little')
    # (4byte)
    o += (sampleRate).to_bytes(4, 'little')
    # (4byte)
    o += (sampleRate * num_channels * bitsPerSample // 8).to_bytes(4, 'little')
    # (2byte)
    o += (num_channels * bitsPerSample // 8).to_bytes(2, 'little')
    # (2byte)
    o += (bitsPerSample).to_bytes(2, 'little')
    # (4byte) Data Chunk Marker
    o += bytes("data", 'ascii')
    # (4byte) Data size in bytes
    o += (datasize).to_bytes(4, 'little')
    return o
