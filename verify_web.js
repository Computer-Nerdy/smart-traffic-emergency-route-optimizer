/**
 * verify_web.js
 * Automated Verification Script for SRM Smart Traffic Web Application
 * Course: 21CSC201J - Data Structures and Algorithms | SRM IST Ramapuram
 */

const fs = require('fs');
const path = require('path');

console.log('================================================================');
console.log('           STARTING AUTOMATED VERIFICATION OF WEB SUITE          ');
console.log('================================================================\n');

let passCount = 0;
let totalChecks = 0;

function assertCheck(desc, condition) {
  totalChecks++;
  if (condition) {
    console.log(`[PASS] ${desc}`);
    passCount++;
  } else {
    console.error(`[FAIL] ${desc}`);
    process.exitCode = 1;
  }
}

// 1. Verify existence and non-zero size of core distribution files
const ROOT = __dirname;
const requiredFiles = [
  'index.html',
  'style.css',
  'app.js',
  'standalone_smart_traffic.c',
  'SRM_21CSC201J_DSA_Project_Report.docx',
  path.join('docs', 'PROJECT_REPORT.html')
];

for (const f of requiredFiles) {
  const p = path.join(ROOT, f);
  const exists = fs.existsSync(p);
  const size = exists ? fs.statSync(p).size : 0;
  assertCheck(`File '${f}' exists with valid non-zero size (${size} bytes)`, exists && size > 500);
}

// 2. Verify HTML contents, elements, and full academic report sections
const htmlContent = fs.readFileSync(path.join(ROOT, 'index.html'), 'utf8');
assertCheck('HTML links to style.css', htmlContent.includes('href="style.css"'));
assertCheck('HTML includes app.js script', htmlContent.includes('src="app.js"'));
assertCheck('HTML contains download link for Word Report', htmlContent.includes('SRM_21CSC201J_DSA_Project_Report.docx'));
assertCheck('HTML contains download link for standalone C', htmlContent.includes('standalone_smart_traffic.c'));
assertCheck('HTML contains link to full HTML project report', htmlContent.includes('docs/PROJECT_REPORT.html'));
assertCheck('HTML contains SVG map container (#city-map-svg)', htmlContent.includes('id="city-map-svg"'));
assertCheck('HTML contains Priority Queue SVG tree container (#heap-tree-svg)', htmlContent.includes('id="heap-tree-svg"'));
assertCheck('HTML contains Turn-by-Turn Telemetry table (#turn-by-turn-tbody)', htmlContent.includes('id="turn-by-turn-tbody"'));
assertCheck('HTML contains 12-test suite terminal (#test-terminal-output)', htmlContent.includes('id="test-terminal-output"'));
assertCheck('HTML contains map action menu container (#map-action-menu)', htmlContent.includes('id="map-action-menu"'));

// Academic Report sections verification in index.html (Requirement 6)
assertCheck('Report viewer contains Bonafide Certificate', htmlContent.includes('BONAFIDE CERTIFICATE'));
assertCheck('Report viewer contains Student Declaration', htmlContent.includes('STUDENT DECLARATION'));
assertCheck('Report viewer contains Table of Contents / Index', htmlContent.includes('INDEX / TABLE OF CONTENTS'));
assertCheck('Report viewer contains Executive Summary & Abstract', htmlContent.includes('EXECUTIVE SUMMARY & ABSTRACT'));
assertCheck('Report viewer contains System Modules Description', htmlContent.includes('SYSTEM ARCHITECTURE & MODULES DESCRIPTION'));
assertCheck('Report viewer contains Algorithmic Flowchart', htmlContent.includes('ALGORITHMIC FLOWCHART'));
assertCheck('Report viewer contains 12-Test Verification Results', htmlContent.includes('AUTOMATED TEST VERIFICATION RESULTS'));
assertCheck('Report viewer contains Performance Benchmarking Table', htmlContent.includes('BENCHMARKING & GREEN CORRIDOR SAVINGS'));
assertCheck('Report viewer contains Conclusion & Future Scope', htmlContent.includes('CONCLUSION & FUTURE SCOPE'));
assertCheck('Report viewer contains Course Completion Certificate', htmlContent.includes('COURSE COMPLETION CERTIFICATE'));

// 3. Verify CSS styling & Print Media Query
const cssContent = fs.readFileSync(path.join(ROOT, 'style.css'), 'utf8');
assertCheck('style.css defines .map-action-menu popover', cssContent.includes('.map-action-menu'));
assertCheck('style.css configures touch-action: none on .map-viewport', cssContent.includes('touch-action: none'));
assertCheck('style.css print query correctly targets #tab-report', cssContent.includes('#tab-report'));
assertCheck('style.css defines .pos-chips-container and .pos-chip', cssContent.includes('.pos-chips-container') && cssContent.includes('.pos-chip'));

// 4. Test Node.js Native Module Export (require('./app.js'))
let appModule;
try {
  appModule = require(path.join(ROOT, 'app.js'));
  assertCheck('app.js exports directly as CommonJS module without "window is not defined" error', 
    appModule && typeof appModule.initGraphData === 'function');
} catch (e) {
  assertCheck('app.js CommonJS export: ' + e.message, false);
}

// 5. Test Algorithmic Engine & Core Data Structures
if (appModule) {
  const { CityGraph, IndexedMinHeap, dijkstraComputeWithTrace, getDijkstraPath, findNearestEmergencyResponder, VertexType } = appModule;

  // Test Min-Heap Ordering
  const heap = new IndexedMinHeap(10);
  heap.insert(0, 50.0);
  heap.insert(1, 10.0);
  heap.insert(2, 30.0);
  const min1 = heap.extractMin();
  assertCheck('IndexedMinHeap extracts true minimum root', min1.vertex === 1 && Math.abs(min1.distance - 10.0) < 1e-6);
  heap.decreaseKey(0, 5.0);
  const min2 = heap.extractMin();
  assertCheck('IndexedMinHeap decrease-key bubbles vertex to root in O(log V)', min2.vertex === 0 && Math.abs(min2.distance - 5.0) < 1e-6);

  // Test Graph & Dijkstra Triangle Detour
  const g = new CityGraph(3);
  g.addVertex(0, 'A', VertexType.INTERSECTION, 0, 0);
  g.addVertex(1, 'B', VertexType.INTERSECTION, 1, 0);
  g.addVertex(2, 'C', VertexType.INTERSECTION, 0.5, 0.5);
  g.addRoad(0, 1, 'Direct', 10.0, 60.0, 2, 'ARTERIAL', false);
  g.addRoad(0, 2, 'Leg1', 2.0, 60.0, 2, 'ARTERIAL', false);
  g.addRoad(2, 1, 'Leg2', 3.0, 60.0, 2, 'ARTERIAL', false);

  const dRes = dijkstraComputeWithTrace(g, 0, false, 'DYNAMIC_TRAVEL_TIME');
  const dPath = getDijkstraPath(dRes, 1);
  assertCheck('Dijkstra selects 2-hop detour path 0->2->1 over direct edge', 
    dPath && dPath.length === 3 && dPath[0] === 0 && dPath[1] === 2 && dPath[2] === 1);
  assertCheck('Dijkstra optimal cost is exactly 5.0 min', Math.abs(dRes.dist[1] - 5.0) < 1e-6);

  // Test Automatic Detour on Road Closure
  g.simulateRoadClosure(0, 2);
  const dResClosure = dijkstraComputeWithTrace(g, 0, false, 'DYNAMIC_TRAVEL_TIME');
  const dPathClosure = getDijkstraPath(dResClosure, 1);
  assertCheck('Dijkstra switches to direct path 0->1 when detour leg is closed',
    dPathClosure && dPathClosure.length === 2 && dPathClosure[0] === 0 && dPathClosure[1] === 1);

  // Test Origin == Destination Edge Case
  const dResSelf = dijkstraComputeWithTrace(g, 0, false, 'DYNAMIC_TRAVEL_TIME');
  const dPathSelf = getDijkstraPath(dResSelf, 0);
  assertCheck('Dijkstra path to self (0->0) correctly returns [0] with 0.0 distance',
    dPathSelf && dPathSelf.length === 1 && dPathSelf[0] === 0 && dResSelf.dist[0] === 0.0);

  // Test Full Chennai Corridor Multi-Depot Dispatch
  const fullGraph = appModule.initGraphData();
  const dispatchHospital = findNearestEmergencyResponder(fullGraph, 3, VertexType.HOSPITAL);
  assertCheck('Multi-depot hospital dispatch to Porur (3) selects MIOT (4) with shortest ETA',
    dispatchHospital && dispatchHospital.bestResponderId === 4 && dispatchHospital.etaMin < 6.0);
  
  const dispatchFire = findNearestEmergencyResponder(fullGraph, 0, VertexType.FIRE_STATION);
  assertCheck('Multi-depot fire dispatch to SRM (0) selects Ashok Nagar Fire Station (7)',
    dispatchFire && dispatchFire.bestResponderId === 7);
}

console.log(`\nVerification Complete: ${passCount} / ${totalChecks} checks PASSED!`);
if (passCount === totalChecks) {
  console.log('STATUS: ALL VERIFICATION CHECKS PASSED WITH 100% SUCCESS RATE!\n');
  process.exit(0);
} else {
  console.error(`STATUS: ${totalChecks - passCount} CHECKS FAILED!\n`);
  process.exit(1);
}
