import time
from enum import IntEnum
from typing import Tuple, List

Nucleotide: IntEnum = IntEnum('Nucleotide', ('A', 'C', 'G', 'T'))
Codon = Tuple[Nucleotide, Nucleotide, Nucleotide]  # type alias for codons
Gene = List[Codon]  # type alias for genes


def string_to_gene(s: str) -> Gene:
    gene: Gene = []
    for i in range(0, len(s), 3):
        if (i + 2) >= len(s):  # don't run off end!
            return gene
        #  initialize codon out of three nucleotides
        codon: Codon = (Nucleotide[s[i]], Nucleotide[s[i + 1]],
                        Nucleotide[s[i + 2]])
        gene.append(codon)  # add codon to gene
    return gene


def linear_contains(gene: Gene, key_codon: Codon) -> bool:
    for codon in gene:
        if codon == key_codon:
            return True
    return False


def binary_contains(gene: Gene, key_codon: Codon) -> bool:
    low = 0
    high = len(gene) - 1
    while low <= high:
        mid = (low + high) // 2
        if gene[mid] < key_codon:
            low = mid + 1
        elif gene[mid] > key_codon:
            high = mid - 1
        else:
            return True
    return False


if __name__ == "__main__":
    gene_str: str = "ACGTGGCTCTCTAACGTACGTACGTACGGGGTTTATATATACCCTAGGACTCCCTTT"
    my_gene: Gene = string_to_gene(gene_str)

    acg: Codon = (Nucleotide.A, Nucleotide.C, Nucleotide.G)
    gat: Codon = (Nucleotide.G, Nucleotide.A, Nucleotide.T)
    # 程序执行时间=cpu时间 + io时间 + 休眠或者等待时间
    # time.clock -- cpu time
    start = time.clock()
    print(linear_contains(my_gene, acg))  # True
    print(linear_contains(my_gene, gat))  # False
    end = time.clock()
    print("Time: ", end - start)

    start = time.clock()
    my_sorted_gene: Gene = sorted(my_gene)
    print(binary_contains(my_sorted_gene, acg))  # True
    print(binary_contains(my_sorted_gene, gat))  # False
    end = time.clock()
    print("Time: ", end - start)
