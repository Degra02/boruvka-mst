def txt_to_dot_with_mst(input_file, mst_file, output_file):
    mst_edges = set()
    with open(mst_file, 'r') as mst_infile:
        mst_infile.readline()
        for line in mst_infile:
            src, dst, _ = map(int, line.strip().split())
            mst_edges.add((min(src, dst), max(src, dst)))

    with open(input_file, 'r') as infile, open(output_file, 'w') as outfile:
        V, E = map(int, infile.readline().strip().split())

        outfile.write('graph G {\n')

        for line in infile:
            src, dst, weight = map(int, line.strip().split())
            edge = (min(src, dst), max(src, dst))
            if edge in mst_edges:
                outfile.write(f'    {src} -- {dst} [label="{weight}", color="red", penwidth=2.0];\n')
            else:
                outfile.write(f'    {src} -- {dst} [label="{weight}"];\n')

        outfile.write('}\n')


txt_to_dot_with_mst('graph.txt', 'mst.txt', 'graph.dot')

