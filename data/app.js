var timers = [];
var pumpStatus = false; // false = OFF, true = ON

// Function to toggle pump on/off
function togglePump() {
    // Request server to toggle pump state
    setPumpState(!pumpStatus);
}

// Function to get color based on value from highlights
function getColorForValue(gauge, value) {
    if (gauge.options.highlights && Array.isArray(gauge.options.highlights)) {
        for (let highlight of gauge.options.highlights) {
            if (value >= highlight.from && value < highlight.to) {
                return highlight.color;
            }
        }
    }
    return gauge.options.colorBarProgress; // fallback to default color
}

function animateGauges() {
    document.gauges.forEach(function(gauge) {
        timers.push(setInterval(function() {
            gauge.value = Math.random() *
                (gauge.options.maxValue - gauge.options.minValue) +
                gauge.options.minValue;
            
            // Update progress bar color based on highlights
            gauge.options.colorBarProgress = getColorForValue(gauge, gauge.value);
        }, gauge.animation.duration + 50));
    });
}

// Protection controls state
var dryRunEnabled = false;
var tankFullEnabled = false;
var overloadEnabled = false;

function toggleDryRun() {
    dryRunEnabled = !dryRunEnabled;
    const btn = document.getElementById('dryrunToggle');
    if (dryRunEnabled) {
        btn.textContent = 'Disable';
        btn.classList.add('active');
    } else {
        btn.textContent = 'Enable';
        btn.classList.remove('active');
    }
}

function toggleTankFull() {
    tankFullEnabled = !tankFullEnabled;
    const btn = document.getElementById('tankToggle');
    if (tankFullEnabled) {
        btn.textContent = 'Disable';
        btn.classList.add('active');
    } else {
        btn.textContent = 'Enable';
        btn.classList.remove('active');
    }
}

function toggleOverload() {
    overloadEnabled = !overloadEnabled;
    const btn = document.getElementById('overloadToggle');
    if (overloadEnabled) {
        btn.textContent = 'Disable';
        btn.classList.add('active');
    } else {
        btn.textContent = 'Enable';
        btn.classList.remove('active');
    }
}

// Update handlers for dashboard and LED style selections
function updateDashboard() {
    const sel = document.getElementById('dashboardSelect');
    const btn = document.getElementById('dashboardUpdateBtn');
    if (!sel || !btn) return;
    const payload = { dashboard_style: parseInt(sel.value || 1, 10) };
    saveConfigToServer(payload, function(err){
        if (btn) { 
            btn.textContent = err ? 'Error' : 'Updated'; 
            setTimeout(()=>btn.textContent='Update',1200); 
        }
    });
}

function updateLedStyle() {
    const sel = document.getElementById('ledStyleSelect');
    const btn = document.getElementById('ledStyleUpdateBtn');
    if (!sel || !btn) return;
    const payload = { led_strip_style: parseInt(sel.value || 1, 10) };
    saveConfigToServer(payload, function(err){
        if (btn) { 
            btn.textContent = err ? 'Error' : 'Updated'; 
            setTimeout(()=>btn.textContent='Update',1200); 
        }
    });
}

function updateDryRunProtection() {
    clampNumberInput('dryrunCutoff', 0, 9999);
    clampNumberInput('dryrunDelay', 0, 60);
    const powerValue = parseInt(document.getElementById('dryrunCutoff')?.value || 0, 10);
    const payload = {
        dry_run_cutoff_protection: dryRunEnabled ? 1 : 0,
        dry_run_cutoff_power_1: Math.floor(powerValue / 1000) % 10,
        dry_run_cutoff_power_2: Math.floor(powerValue / 100) % 10,
        dry_run_cutoff_power_3: Math.floor(powerValue / 10) % 10,
        dry_run_cutoff_power_4: powerValue % 10,
        dry_run_cutoff_delay: parseInt(document.getElementById('dryrunDelay')?.value || 0, 10)
    };
    saveConfigToServer(payload, function(err){
        const btn = document.getElementById('dryrunUpdateBtn'); if (btn) { btn.textContent = err ? 'Error' : 'Updated'; setTimeout(()=>btn.textContent='Update',1200); }
    });
}

function updateTankProtection() {
    clampNumberInput('tankCutoff', 1, 8);
    clampNumberInput('tankDelay', 0, 200);
    const payload = {
        tank_full_cutoff_protection: tankFullEnabled ? 1 : 0,
        tank_full_cutoff_level: parseInt(document.getElementById('tankCutoff')?.value || 1, 10),
        tank_full_cutoff_delay: parseInt(document.getElementById('tankDelay')?.value || 0, 10)
    };
    saveConfigToServer(payload, function(err){
        const btn = document.getElementById('tankUpdateBtn'); if (btn) { btn.textContent = err ? 'Error' : 'Updated'; setTimeout(()=>btn.textContent='Update',1200); }
    });
}

function updateOverloadProtection() {
    clampNumberInput('overloadCutoff', 0, 9999);
    clampNumberInput('overloadDelay', 0, 60);
    const powerValue = parseInt(document.getElementById('overloadCutoff')?.value || 0, 10);
    const payload = {
        overload_cutoff_protection: overloadEnabled ? 1 : 0,
        overload_cutoff_power_1: Math.floor(powerValue / 1000) % 10,
        overload_cutoff_power_2: Math.floor(powerValue / 100) % 10,
        overload_cutoff_power_3: Math.floor(powerValue / 10) % 10,
        overload_cutoff_power_4: powerValue % 10,
        overload_cutoff_delay: parseInt(document.getElementById('overloadDelay')?.value || 0, 10)
    };
    saveConfigToServer(payload, function(err){
        const btn = document.getElementById('overloadUpdateBtn'); if (btn) { btn.textContent = err ? 'Error' : 'Updated'; setTimeout(()=>btn.textContent='Update',1200); }
    });
}

// Input validation helpers
function clampNumberInput(id, min, max) {
    const el = document.getElementById(id);
    if (!el) return;
    let v = parseInt(el.value, 10);
    if (isNaN(v)) v = min;
    if (v < min) v = min;
    if (v > max) v = max;
    el.value = v;
}

// Attach simple onblur validation
document.addEventListener('DOMContentLoaded', function() {
    const checks = [
        ['dryrunCutoff', 0, 9999],
        ['dryrunDelay', 0, 60],
        ['tankCutoff', 1, 8],
        ['tankDelay', 0, 200],
        ['overloadCutoff', 1200, 9999],
        ['overloadDelay', 0, 60]
    ];
    checks.forEach(function(c) {
        const el = document.getElementById(c[0]);
        if (el) el.addEventListener('blur', function() { clampNumberInput(c[0], c[1], c[2]); });
    });
    // Wire UI buttons to API actions
    const pumpBtn = document.getElementById('pumpToggle');
    if (pumpBtn) pumpBtn.addEventListener('click', function(e) { e.preventDefault(); togglePump(); });

    const dryUpdate = document.getElementById('dryrunUpdateBtn');
    if (dryUpdate) dryUpdate.addEventListener('click', updateDryRunProtectionAndSave);
    const tankUpdate = document.getElementById('tankUpdateBtn');
    if (tankUpdate) tankUpdate.addEventListener('click', updateTankProtectionAndSave);
    const overloadUpdate = document.getElementById('overloadUpdateBtn');
    if (overloadUpdate) overloadUpdate.addEventListener('click', updateOverloadProtectionAndSave);
    const dashUpdate = document.getElementById('dashboardUpdateBtn');
    if (dashUpdate) dashUpdate.addEventListener('click', updateDashboardAndSave);
    const ledUpdate = document.getElementById('ledStyleUpdateBtn');
    if (ledUpdate) ledUpdate.addEventListener('click', updateLedStyleAndSave);

    // Initial fetch
    fetchSensors();
    fetchConfig();
    fetchDevice();
    fetchNetwork();

    // Poll sensors periodically
    setInterval(fetchSensors, 2000);
    setInterval(fetchConfig, 10000);
});

// --- API integration functions ---
function setPumpState(state) {
    fetch('/api/pump', {
        method: 'POST',
        headers: {'Content-Type':'application/json'},
        body: JSON.stringify({state: !!state})
    }).then(function(res) {
        if (!res.ok) throw new Error('Pump API error');
        return res.json().catch(() => ({}));
    }).then(function() {
        pumpStatus = !!state;
        const toggleBtn = document.getElementById('pumpToggle');
        const statusText = document.getElementById('statusText');
        if (pumpStatus) {
            if (toggleBtn) { toggleBtn.textContent = 'Turn Off'; toggleBtn.classList.add('active'); }
            if (statusText) { statusText.textContent = 'ON'; statusText.classList.remove('off'); statusText.classList.add('on'); }
        } else {
            if (toggleBtn) { toggleBtn.textContent = 'Turn On'; toggleBtn.classList.remove('active'); }
            if (statusText) { statusText.textContent = 'OFF'; statusText.classList.remove('on'); statusText.classList.add('off'); }
        }
    }).catch(function(err){
        console.error(err);
    });
}

function fetchSensors() {
    fetch('/api/sensors').then(r=>r.json()).then(function(data){
        // Update basic UI elements if present
        if (typeof data !== 'object') return;
        if (data.hasOwnProperty('pump_running')) {
            pumpStatus = !!data.pump_running;
            const statusText = document.getElementById('statusText');
            const toggleBtn = document.getElementById('pumpToggle');
            if (pumpStatus) {
                if (statusText) { statusText.textContent = 'ON'; statusText.classList.remove('off'); statusText.classList.add('on'); }
                if (toggleBtn) { toggleBtn.textContent = 'Turn Off'; toggleBtn.classList.add('active'); }
            } else {
                if (statusText) { statusText.textContent = 'OFF'; statusText.classList.remove('on'); statusText.classList.add('off'); }
                if (toggleBtn) { toggleBtn.textContent = 'Turn On'; toggleBtn.classList.remove('active'); }
            }
        }
        if (data.hasOwnProperty('todayEnergy')) {
            const kwh = document.getElementById('kwhValue');
            if (kwh) kwh.textContent = parseFloat(data.todayEnergy || 0).toFixed(2);
        }
        // Update radial gauges (Power, Voltage, Current)
        if (document.gauges) {
            // Update watts gauge
            if (data.hasOwnProperty('power')) {
                const pumpGauge = document.gauges[0]; // First radial gauge
                if (pumpGauge) pumpGauge.value = parseFloat(data.power) || 0;
            }
            // Update voltage gauge
            if (data.hasOwnProperty('voltage')) {
                const voltGauge = document.gauges[1]; // Second radial gauge
                if (voltGauge) voltGauge.value = parseFloat(data.voltage) || 0;
            }
            // Update current gauge
            if (data.hasOwnProperty('current')) {
                const currentGauge = document.gauges[2]; // Third radial gauge
                if (currentGauge) currentGauge.value = parseFloat(data.current) || 0;
            }
            // Update flow gauge (linear)
            if (data.hasOwnProperty('currentLPM')) {
                const flowGauge = document.gauges[3]; // Fourth gauge (flow linear)
                if (flowGauge) flowGauge.value = parseFloat(data.currentLPM) || 0;
            }
        }
        // Update tank water level gauges
        if (data.hasOwnProperty('waterLevel') && Array.isArray(data.waterLevel)) {
            const tankGauges = [
                { id: 'tankGauge1', levelIndex: 0 },
                { id: 'tankGauge2', levelIndex: 1 },
                { id: 'tankGauge3', levelIndex: 2 }
            ];
            tankGauges.forEach(tank => {
                const canvas = document.getElementById(tank.id);
                if (canvas && document.gauges) {
                    const gauge = document.gauges.find(g => g.canvas === canvas);
                    if (gauge && data.waterLevel[tank.levelIndex] !== undefined) {
                        gauge.value = data.waterLevel[tank.levelIndex];
                    }
                }
            });
        }
        // Log other sensor values for future mapping to gauges
        console.log('Sensors:', data);
    }).catch(err=>console.error('fetchSensors', err));
}

function fetchConfig() {
    fetch('/api/config').then(r=>r.json()).then(function(cfg){
        if (!cfg || typeof cfg !== 'object') return;
        // Dry run
        if (cfg.hasOwnProperty('dry_run_cutoff_protection')) {
            const checkbox = document.getElementById('dryrunToggle');
            dryRunEnabled = !!cfg.dry_run_cutoff_protection;
            if (checkbox) {
                checkbox.checked = dryRunEnabled;
            }
        }
        // Reconstruct dry_run_cutoff power from 4 separate digits
        if (cfg.hasOwnProperty('dry_run_cutoff_power_1') && cfg.hasOwnProperty('dry_run_cutoff_power_2') &&
            cfg.hasOwnProperty('dry_run_cutoff_power_3') && cfg.hasOwnProperty('dry_run_cutoff_power_4')) {
            const el = document.getElementById('dryrunCutoff');
            const powerValue = (cfg.dry_run_cutoff_power_1 * 1000) + (cfg.dry_run_cutoff_power_2 * 100) + 
                              (cfg.dry_run_cutoff_power_3 * 10) + cfg.dry_run_cutoff_power_4;
            if (el) el.value = powerValue;
        }
        if (cfg.hasOwnProperty('dry_run_cutoff_delay')) {
            const el = document.getElementById('dryrunDelay'); if (el) el.value = cfg.dry_run_cutoff_delay;
        }

        // Tank
        if (cfg.hasOwnProperty('tank_full_cutoff_protection')) {
            const checkbox = document.getElementById('tankToggle');
            tankFullEnabled = !!cfg.tank_full_cutoff_protection;
            if (checkbox) {
                checkbox.checked = tankFullEnabled;
            }
        }
        if (cfg.hasOwnProperty('tank_full_cutoff_level')) {
            const el = document.getElementById('tankCutoff'); if (el) el.value = cfg.tank_full_cutoff_level;
        }
        if (cfg.hasOwnProperty('tank_full_cutoff_delay')) {
            const el = document.getElementById('tankDelay'); if (el) el.value = cfg.tank_full_cutoff_delay;
        }

        // Overload
        if (cfg.hasOwnProperty('overload_cutoff_protection')) {
            const checkbox = document.getElementById('overloadToggle');
            overloadEnabled = !!cfg.overload_cutoff_protection;
            if (checkbox) {
                checkbox.checked = overloadEnabled;
            }
        }
        // Reconstruct overload_cutoff power from 4 separate digits
        if (cfg.hasOwnProperty('overload_cutoff_power_1') && cfg.hasOwnProperty('overload_cutoff_power_2') &&
            cfg.hasOwnProperty('overload_cutoff_power_3') && cfg.hasOwnProperty('overload_cutoff_power_4')) {
            const el = document.getElementById('overloadCutoff');
            const powerValue = (cfg.overload_cutoff_power_1 * 1000) + (cfg.overload_cutoff_power_2 * 100) + 
                              (cfg.overload_cutoff_power_3 * 10) + cfg.overload_cutoff_power_4;
            if (el) el.value = powerValue;
        }
        if (cfg.hasOwnProperty('overload_cutoff_delay')) {
            const el = document.getElementById('overloadDelay'); if (el) el.value = cfg.overload_cutoff_delay;
        }

        // Dashboard and LED style
        if (cfg.hasOwnProperty('dashboard_style')) {
            const el = document.getElementById('dashboardSelect'); if (el) el.value = cfg.dashboard_style;
        }
        if (cfg.hasOwnProperty('led_strip_style')) {
            const el = document.getElementById('ledStyleSelect'); if (el) el.value = cfg.led_strip_style;
        }

        console.log('Config loaded', cfg);
    }).catch(err=>console.error('fetchConfig', err));
}

function fetchDevice() {
    fetch('/api/device').then(r=>r.json()).then(function(d){ console.log('Device:', d); }).catch(()=>{});
}

function fetchNetwork() {
    fetch('/api/network').then(r=>r.json()).then(function(n){ console.log('Network:', n); }).catch(()=>{});
}

function saveConfigToServer(payload, cb) {
    fetch('/api/config', {
        method: 'POST', headers: {'Content-Type':'application/json'}, body: JSON.stringify(payload)
    }).then(function(res){ if (!res.ok) throw new Error('config save failed'); return res.json().catch(()=>({})); })
    .then(function(){ if (cb) cb(null); fetchConfig(); })
    .catch(function(err){ console.error(err); if (cb) cb(err); });
}

function updateDryRunProtectionAndSave() {
    updateDryRunProtection();
}

function updateTankProtectionAndSave() {
    updateTankProtection();
}

function updateOverloadProtectionAndSave() {
    updateOverloadProtection();
}

function updateDashboardAndSave() {
    updateDashboard();
}

function updateLedStyleAndSave() {
    updateLedStyle();
}

// Optional: send system command (e.g., reset)
function sendSystemCommand(command) {
    fetch('/api/system', { method: 'POST', headers:{'Content-Type':'application/json'}, body: JSON.stringify({command: command}) })
    .then(r=>r.json().catch(()=>({}))).then(function(){ console.log('System command sent', command); }).catch(err=>console.error(err));
}







