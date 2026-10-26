import os
import matplotlib.pyplot as plt
import matplotlib.patches as patches

def generate_clean_flowchart():
    # Set high resolution figure (18 x 21 inches at 300 DPI)
    fig = plt.figure(figsize=(18, 21), dpi=300)
    ax = fig.add_subplot(111)
    ax.set_xlim(0, 100)
    ax.set_ylim(0, 114)
    ax.axis('off')

    # Color Palette - Professional Clean Engineering Theme
    c_bg = '#f8fafc'
    c_navy = '#0f172a'
    c_blue = '#2563eb'
    c_blue_light = '#eff6ff'
    c_emerald = '#059669'
    c_emerald_light = '#ecfdf5'
    c_amber = '#d97706'
    c_amber_light = '#fffbeb'
    c_crimson = '#dc2626'
    c_crimson_light = '#fef2f2'
    c_purple = '#6366f1'
    c_purple_light = '#f5f3ff'
    c_slate = '#475569'
    c_border = '#cbd5e1'
    c_text_dark = '#0f172a'

    fig.patch.set_facecolor(c_bg)

    # Helper function to draw rounded boxes with text
    def draw_box(x, y, w, h, title, subtitle="", bg='#ffffff', border=c_border, txt_col=c_text_dark, border_w=1.8, rounding=1.2, z=3):
        # Drop shadow
        shadow = patches.FancyBboxPatch((x + 0.4, y - 0.4), w, h, boxstyle=f"round,pad=0.2,rounding_size={rounding}",
                                        facecolor='#94a3b8', edgecolor='none', alpha=0.22, zorder=z-1)
        ax.add_patch(shadow)
        # Main box
        box = patches.FancyBboxPatch((x, y), w, h, boxstyle=f"round,pad=0.2,rounding_size={rounding}",
                                     facecolor=bg, edgecolor=border, linewidth=border_w, zorder=z)
        ax.add_patch(box)
        if subtitle:
            ax.text(x + w/2, y + h*0.64, title, ha='center', va='center', color=txt_col,
                    fontsize=10.2, fontweight='bold', family='sans-serif', zorder=z+1)
            ax.text(x + w/2, y + h*0.28, subtitle, ha='center', va='center', color=c_slate,
                    fontsize=8.4, family='sans-serif', zorder=z+1)
        else:
            ax.text(x + w/2, y + h/2, title, ha='center', va='center', color=txt_col,
                    fontsize=10.2, fontweight='bold', family='sans-serif', zorder=z+1)

    # Helper function to draw oval (Start / End)
    def draw_oval(x, y, w, h, text, bg=c_navy, txt_col='#ffffff'):
        shadow = patches.FancyBboxPatch((x + 0.4, y - 0.4), w, h, boxstyle="round,pad=0.2,rounding_size=3.5",
                                        facecolor='#94a3b8', edgecolor='none', alpha=0.22, zorder=2)
        ax.add_patch(shadow)
        oval = patches.FancyBboxPatch((x, y), w, h, boxstyle="round,pad=0.2,rounding_size=3.5",
                                      facecolor=bg, edgecolor='#1e293b', linewidth=2, zorder=3)
        ax.add_patch(oval)
        ax.text(x + w/2, y + h/2, text, ha='center', va='center', color=txt_col,
                fontsize=11, fontweight='bold', family='sans-serif', zorder=4)

    # Helper function to draw decision diamonds
    def draw_diamond(cx, cy, w, h, title, subtitle="", bg=c_amber_light, border=c_amber):
        pts = [[cx, cy + h/2], [cx + w/2, cy], [cx, cy - h/2], [cx - w/2, cy]]
        shadow_pts = [[cx+0.4, cy + h/2 - 0.4], [cx + w/2 + 0.4, cy - 0.4], [cx + 0.4, cy - h/2 - 0.4], [cx - w/2 + 0.4, cy - 0.4]]
        ax.add_patch(patches.Polygon(shadow_pts, facecolor='#94a3b8', edgecolor='none', alpha=0.20, zorder=2))
        ax.add_patch(patches.Polygon(pts, facecolor=bg, edgecolor=border, linewidth=2, zorder=3))
        if subtitle:
            ax.text(cx, cy + 1.8, title, ha='center', va='center', color=c_text_dark,
                    fontsize=9.6, fontweight='bold', family='sans-serif', zorder=4)
            ax.text(cx, cy - 1.4, subtitle, ha='center', va='center', color=c_slate,
                    fontsize=8.2, family='sans-serif', zorder=4)
        else:
            ax.text(cx, cy, title, ha='center', va='center', color=c_text_dark,
                    fontsize=9.6, fontweight='bold', family='sans-serif', zorder=4)

    # Helper function to draw connecting arrows
    def draw_arrow(x1, y1, x2, y2, label="", col='#334155', rad=0.0, lw=2.0, lcol='#1e293b'):
        ax.annotate('', xy=(x2, y2), xytext=(x1, y1),
                    arrowprops=dict(arrowstyle="-|>", color=col, lw=lw,
                                    connectionstyle=f"arc3,rad={rad}", shrinkA=3, shrinkB=4),
                    zorder=2)
        if label:
            mx = (x1 + x2) / 2
            my = (y1 + y2) / 2
            ax.text(mx, my + 1.2, label, ha='center', va='center', fontsize=8.5,
                    fontweight='bold', color=lcol, backgroundcolor=c_bg, zorder=5)

    # ----------------- PIPELINE ELEMENTS (NO SRM / NO HEADERS) -----------------

    # Top: START
    draw_oval(36, 104.5, 28, 5.2, "START: System Launch & Initialization", bg=c_navy)

    # Node 2: Initialize City Graph
    draw_box(25, 93.5, 50, 7.0, "Module 1: Initialize City Road Network Graph",
             "Adjacency List Structure | V Vertices (Intersections & Hubs) | E Directed Links",
             bg='#ffffff', border=c_blue)
    draw_arrow(50, 104.5, 50, 100.5)

    # Node 3: Ingest IoT Sensor Data
    draw_box(25, 82.5, 50, 7.0, "Module 2: Ingest Roadside IoT Sensor Telemetry",
             "Live Speed Radar, Traffic Density, Monsoon Waterlogging, Incident Flags",
             bg=c_emerald_light, border=c_emerald)
    draw_arrow(50, 93.5, 50, 89.5)

    # Node 4: Dynamic Edge Cost Computation
    draw_box(20, 71.5, 60, 7.0, "Dynamic Travel Time Weight Calculation",
             "Cost(u,v) = [(Distance / SpeedLimit) × 60] × CongestionFactor + IncidentPenalty",
             bg='#ffffff', border=c_navy)
    draw_arrow(50, 82.5, 50, 78.5)

    # Node 5: Decision Diamond - Mode Selection (w=50, h=13.0)
    draw_diamond(50, 60.5, 50, 13.0, "Evaluate Operational Routing Request",
                 "Determine vehicle profile, priority class, and dispatch requirements",
                 bg=c_amber_light, border=c_amber)
    draw_arrow(50, 71.5, 50, 67.0)

    # Three Major Tracks: Left = Civilian Track, Center = Emergency Track, Right = Regression Suite
    draw_arrow(33, 60.5, 17, 52.5, label="Civilian Vehicle", rad=0.15)
    draw_arrow(50, 54.0, 50, 52.5, label="Emergency Vehicle", rad=0.0)
    draw_arrow(67, 60.5, 83, 52.5, label="Verification Test", rad=-0.15)

    # ================= TRACK 1: CIVILIAN ROUTING (LEFT) =================
    draw_box(4, 45.5, 26, 7.0, "Input Origin & Destination",
             "User queries Origin (S) and Destination (T)\nfrom dropdown or interactive map click",
             bg='#ffffff', border=c_blue)

    draw_box(4, 34.5, 26, 7.0, "Initialize Min-Heap Priority Queue",
             "pos[S] = 0, dist[S] = 0.0\npos[v] = -1, dist[v] = INF for all v != S",
             bg=c_blue_light, border=c_blue)
    draw_arrow(17, 45.5, 17, 41.5)

    draw_box(4, 23.5, 26, 7.0, "Extract-Min Operation",
             "u = min_heap_extract_min(heap) in O(log V)\nMark visited[u] = true",
             bg='#ffffff', border=c_blue)
    draw_arrow(17, 34.5, 17, 30.5)

    draw_diamond(17, 13.5, 30, 11.0, "Is u == Destination T\nor Heap Empty?",
                 "Termination Check", bg=c_amber_light, border=c_amber)
    draw_arrow(17, 23.5, 17, 19.0)

    # Edge Relaxation Box
    draw_box(4, 1.2, 26, 8.5, "Relax Outgoing Edges (u -> v)",
             "If dist[u] + Cost(u,v) < dist[v]:\n  dist[v] = dist[u] + Cost(u,v)\n  parent[v] = u\n  min_heap_decrease_key(v, dist[v]) in O(log V)",
             bg='#ffffff', border=c_blue)

    # Arrow from Decision "No" to Relaxation
    draw_arrow(17, 8.0, 17, 9.7, label="No (Explore)")

    # Arrow from Relaxation back up to Extract-Min (loop back)
    ax.annotate('', xy=(4, 27), xytext=(4, 5.4),
                arrowprops=dict(arrowstyle="-|>", color=c_blue, lw=1.8,
                                connectionstyle="arc3,rad=-0.4", shrinkA=2, shrinkB=2), zorder=2)
    ax.text(1.2, 16.0, "Next Step", ha='center', va='center', fontsize=8, color=c_blue, fontweight='bold', rotation=90)

    # ================= TRACK 2: EMERGENCY DISPATCH & GREEN CORRIDOR (CENTER) =================
    draw_box(37, 45.5, 26, 7.0, "Incident Scene Identification",
             "Locate emergency incident intersection\n(Accident, Medical Trauma, Fire Outbreak)",
             bg='#ffffff', border=c_crimson)

    draw_box(37, 34.5, 26, 7.0, "Multi-Depot Priority Search",
             "Query all Trauma Centers & Fire Stations\nCompute Dijkstra arrival ETA from each depot",
             bg=c_crimson_light, border=c_crimson)
    draw_arrow(50, 45.5, 50, 41.5)

    draw_box(37, 23.5, 26, 7.0, "Select Optimal Depot D*",
             "Select depot with global minimum ETA:\nD* = argmin { TravelTime(Di -> Target) }",
             bg='#ffffff', border=c_crimson)
    draw_arrow(50, 34.5, 50, 30.5)

    draw_box(37, 13.0, 26, 6.8, "Smart Green Corridor Preemption",
             "Downstream signal preemption engaged\nFree-flow multiplier = 0.85x (-68.3% rush delay)",
             bg=c_emerald_light, border=c_emerald)
    draw_arrow(50, 23.5, 50, 19.8)

    draw_box(37, 1.8, 26, 7.5, "Dispatch Emergency Responder",
             "Trigger siren beacons, vehicle deployment\nand real-time IoT clearance wave along route",
             bg='#ffffff', border=c_emerald)
    draw_arrow(50, 13.0, 50, 9.3)

    # ================= TRACK 3: 12-TEST AUTOMATED VERIFICATION (RIGHT) =================
    draw_box(70, 45.5, 26, 7.0, "Execute 12-Test Suite",
             "Automated regression verification\ncovering algorithmic and memory invariants",
             bg='#ffffff', border=c_purple)

    draw_box(70, 32.5, 26, 9.2, "Assert Invariants",
             "[1] Min-Heap O(log V) correctness\n[2] Triangle inequality detours\n[3] Dynamic road closures (INF cost)\n[4] Zero-cost cycle resilience",
             bg=c_purple_light, border=c_purple)
    draw_arrow(83, 45.5, 83, 41.7)

    draw_box(70, 20.0, 26, 7.5, "Assert Boundary Conditions",
             "[5] Disconnected graph handling\n[6] Dynamic realloc capacity growth\n[7] Multi-depot ETA tie-breaking\n[8] Input bounds sanitization",
             bg='#ffffff', border=c_purple)
    draw_arrow(83, 32.5, 83, 27.5)

    draw_box(70, 6.8, 26, 7.5, "Verify 100% Pass Rate",
             "Output validation report to virtual terminal,\nC test binary, and web dashboard",
             bg=c_emerald_light, border=c_emerald)
    draw_arrow(83, 20.0, 83, 14.3)

    # Save to docs and artifacts
    os.makedirs('docs', exist_ok=True)
    out_docs = os.path.join('docs', 'clean_flowchart.png')
    plt.savefig(out_docs, dpi=300, bbox_inches='tight', facecolor=fig.get_facecolor(), edgecolor='none')
    print(f"Clean flowchart saved to {out_docs}")

    # Also overwrite system_flowchart.png
    out_docs_sys = os.path.join('docs', 'system_flowchart.png')
    plt.savefig(out_docs_sys, dpi=300, bbox_inches='tight', facecolor=fig.get_facecolor(), edgecolor='none')

    # Also save to artifacts directory
    artifact_dir = r'C:\Users\91807\.gemini\antigravity\brain\d69874dc-bb70-41bf-b8b0-0e09de54be33'
    if os.path.exists(artifact_dir):
        out_art1 = os.path.join(artifact_dir, 'clean_flowchart.png')
        out_art2 = os.path.join(artifact_dir, 'system_flowchart.png')
        plt.savefig(out_art1, dpi=300, bbox_inches='tight', facecolor=fig.get_facecolor(), edgecolor='none')
        plt.savefig(out_art2, dpi=300, bbox_inches='tight', facecolor=fig.get_facecolor(), edgecolor='none')
        print(f"Clean flowchart saved to {out_art1} and {out_art2}")

    plt.close()

if __name__ == '__main__':
    generate_clean_flowchart()
