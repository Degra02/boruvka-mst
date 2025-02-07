import os
import subprocess

def txt_to_dot_with_mst(input_file, mst_file, output_dir, layouts, label_fontsize=12, node_distance=1.0):
    os.makedirs(output_dir, exist_ok=True)

    mst_edges = set()
    with open(mst_file, 'r') as mst_infile:
        mst_infile.readline()
        for line in mst_infile:
            src, dst, _ = map(int, line.strip().split())
            mst_edges.add((min(src, dst), max(src, dst)))

    for layout in layouts:
        output_dot_file = os.path.join(output_dir, f'graph_{layout}.dot')
        output_svg = os.path.join(output_dir, f'graph_{layout}.svg')

        with open(input_file, 'r') as infile, open(output_dot_file, 'w') as outfile:
            V, E = map(int, infile.readline().strip().split())

            outfile.write(f'graph G {{\n')
            outfile.write(f'    layout="{layout}";\n')
            outfile.write(f'    nodesep={node_distance};\n')
            outfile.write(f'    ranksep={node_distance};\n')
            
            for line in infile:
                src, dst, weight = map(int, line.strip().split())
                edge = (min(src, dst), max(src, dst))
                if edge in mst_edges:
                    outfile.write(f'    {src} -- {dst} [label="{weight}", fontsize={label_fontsize}, color="red", penwidth=2.0];\n')
                else:
                    outfile.write(f'    {src} -- {dst} [label="{weight}", fontsize={label_fontsize}];\n')
            
            outfile.write('}\n')

        try:
            subprocess.run(['dot', '-Tsvg', output_dot_file, '-o', output_svg], check=True)
            print(f"Generated {output_svg}")
        except subprocess.CalledProcessError as e:
            print(f"Error generating SVG for layout {layout}: {e}")


layouts = ["dot", "neato", "fdp", "sfdp", "twopi", "circo", "osage"]
txt_to_dot_with_mst(
    input_file='graph.txt', 
    mst_file='mst.txt', 
    output_dir='graph_images', 
    layouts=layouts, 
    label_fontsize=5,
    node_distance=0.5
)

