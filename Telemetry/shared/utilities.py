"""
Utility functions
"""

import numpy as np
from datetime import datetime

NUM_BYTES_IN_WORD = 4

def calculate_crc(crc_bytes, payload_length, use_numpy=True):
    """
    Calculate the crc from a 4-byte aligned array
    crc_bytes: numpy array of bytes or list of bytes depending on use_numpy
    payload_length: How many bytes make up the payload
    """
    
    num_elements = len(crc_bytes[0:4+payload_length])
    if (num_elements % NUM_BYTES_IN_WORD != 0):
        return np.zeros(4) if use_numpy else [0, 0, 0, 0]

    num_words = int(num_elements / NUM_BYTES_IN_WORD)

    # swap endians
    uint32_value = [((crc_bytes[i]) + (crc_bytes[i+1] << 8) + (crc_bytes[i+2] << 16) + (crc_bytes[i+3] << 24)) for i in range(0, num_elements, 4)]

    raw = crc32Block(0xFFFFFFFF, num_words, uint32_value)
    raw = raw ^ 0xFFFFFFFF
    
    if use_numpy:
        crc = np.array([(raw >> 24) & 0xFF, (raw >> 16) & 0xFF, (raw >> 8) & 0xFF, raw & 0xFF])
    else:
        crc = [(raw >> 24) & 0xFF, (raw >> 16) & 0xFF, (raw >> 8) & 0xFF, raw & 0xFF]
    return crc

def crc32Block(crc, size, data):
    for i in range(size):
        crc = crc32(crc, data[i])
        
    return crc

def crc32(crc, data):
    crc = crc ^ data
    for i in range(32):
        if crc & 0x80000000:
            crc = (crc << 1) ^ 0x04C11DB7
        else:
            crc = crc << 1

    return crc
